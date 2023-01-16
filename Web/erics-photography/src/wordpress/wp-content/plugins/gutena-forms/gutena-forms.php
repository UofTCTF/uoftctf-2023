<?php
/**
 * Plugin Name:       Gutena Forms - Contact Forms Block
 * Description:       Gutena Forms is the easiest way to create forms inside the WordPress block editor. Our plugin does not use jQuery and is lightweight, so you can rest assured that it won’t slow down your website. Instead, it allows you to quickly and easily create custom forms right inside the block editor.
 * Requires at least: 6.0
 * Requires PHP:      5.6
 * Version:           1.0.5
 * Author:            ExpressTech
 * Author URI:        https://expresstech.io
 * License:           GPL-2.0-or-later
 * License URI:       https://www.gnu.org/licenses/gpl-2.0.html
 * Text Domain:       gutena-forms
 *
 * @package           gutena-forms
 */


defined( 'ABSPATH' ) || exit;

/**
 * Abort if the class is already exists.
 */
if ( ! class_exists( 'Gutena_Forms' ) ) {

	class Gutena_Forms {

		public $version = '1.0.5';
		// The instance of this class
		private static $instance = null;

		// Returns the instance of this class.
		public static function get_instance() {
			if ( null === self::$instance ) {
				self::$instance = new self();
			}
			return self::$instance;
		}

		public function __construct() {
			add_action( 'init', array( $this, 'register_blocks_and_scripts' ) );
			add_filter( 'block_categories_all', array( $this, 'register_category' ), 10, 2 );
			add_action( 'save_post', array( $this, 'save_gutena_forms_schema' ), 10, 3 );

			add_action( 'wp_ajax_gutena_forms_submit', array( $this, 'submit_form' ) );
			add_action( 'wp_ajax_nopriv_gutena_forms_submit', array( $this, 'submit_form' ) );
		}

		// Register blocks and scripts
		public function register_blocks_and_scripts() {
			if ( ! function_exists( 'register_block_type' ) ) {
				// Block editor is not available.
				return;
			}
			// Guten Forms Block
			register_block_type(
				__DIR__ . '/build',
				array(
					'render_callback' => array( $this, 'render_form' ),
				)
			);

			// Field group Block
			register_block_type( __DIR__ . '/build/field-group' );

			// Form Confirmation Message Block
			register_block_type( __DIR__ . '/build/form-confirm-msg' );

			// Form Error Message Block
			register_block_type( __DIR__ . '/build/form-error-msg' );

			// Form Field Block
			register_block_type(
				__DIR__ . '/build/form-field',
				array(
					'render_callback' => array( $this, 'render_form_field' ),
				)
			);

			wp_localize_script(
				'gutena-forms-script',
				'gutenaFormsBlock',
				array(
					'submit_action'       => 'gutena_forms_submit',
					'ajax_url'            => admin_url( 'admin-ajax.php' ),
					'nonce'               => wp_create_nonce( 'gutena_Forms' ),
					'required_msg'        => __( 'Please fill in this field', 'gutena-forms' ),
					'required_msg_select' => __( 'Please select an option', 'gutena-forms' ),
					'invalid_email_msg'   => __( 'Please enter a valid email address', 'gutena-forms' ),
				)
			);
		}

		// Register Gutena category if not exists
		public function register_category( $block_categories, $editor_context ) {
			$fields = wp_list_pluck( $block_categories, 'slug' );

			if ( ! empty( $editor_context->post ) && ! in_array( 'gutena', $fields, true ) ) {
				array_push(
					$block_categories,
					array(
						'slug'  => 'gutena',
						'title' => __( 'Gutena', 'gutena-forms' ),
					)
				);
			}

			return $block_categories;
		}

		// render_callback : form field
		public function render_form_field( $attributes, $content, $block ) {
			// No changes if fieldType is empty
			if ( empty( $attributes ) || empty( $attributes['fieldType'] ) ) {
				return $content;
			}

			// Get Block Supports like styles or classNames
			$wrapper_attributes = get_block_wrapper_attributes(
				array(
					'class' => 'gutena-forms-' . esc_attr( $attributes['fieldType'] ) . '-field',
				)
			);
			// Output Html
			$output = '';

			$required = empty( $attributes['fieldType'] ) ? '' : 'required';

			// Text type Input
			if ( in_array( $attributes['fieldType'], array( 'text', 'email', 'number', 'hidden', 'tel', 'url' ) ) ) {
				$output = '<input 
				name="' . esc_attr( $attributes['nameAttr'] ) . '" 
				type="' . esc_attr( $attributes['fieldType'] ) . '" 
				 class="' . esc_attr( $attributes['fieldClasses'] ) . '" 
				placeholder="' . esc_attr( $attributes['placeholder'] ) . '" 
				' . esc_attr( $required ) . ' 
				' . ( empty( $attributes['maxlength'] ) ? '' : 'maxlength="' . esc_attr( $attributes['maxlength'] ) . '"' ) . ' />';
			}

			// Textarea type Input
			if ( 'textarea' === $attributes['fieldType'] ) {
				$output = '<textarea 
				name="' . esc_attr( $attributes['nameAttr'] ) . '"  
				class="' . esc_attr( $attributes['fieldClasses'] ) . '" 
				rows="' . esc_attr( $attributes['textAreaRows'] ) . '" 
				' . esc_attr( $required ) . ' 
				placeholder="' . esc_attr( $attributes['placeholder'] ) . '" 
				' . ( empty( $attributes['maxlength'] ) ? '' : 'maxlength="' . esc_attr( $attributes['maxlength'] ) . '"' ) . ' ></textarea>';
			}

			// Select type Input
			if ( 'select' === $attributes['fieldType'] ) {
				$output = '<select  
				class="' . esc_attr( $attributes['fieldClasses'] ) . '"  
				' . esc_attr( $required ) . ' 
				 >';
				if ( ! empty( $attributes['selectOptions'] ) && is_array( $attributes['selectOptions'] ) ) {
					foreach ( $attributes['selectOptions'] as $option ) {
						$output .= '<option value="' . esc_attr( $option ) . '" >' . esc_attr( $option ) . '</option>';
					}
				}
				$output .= '</select>';
			}

			// output
			return sprintf(
				'<div %1$s>%2$s</div>',
				$wrapper_attributes,
				$output
			);
		}

		//Replace last occurance of a string
		public function str_last_replace( $search, $replace, $str ) {
			//finds the position of the last occurrence of a string
			$pos = strripos($str, $search);
		
			if ( $pos !== false ) {
				$str = substr_replace($str, $replace, $pos, strlen($search));
			}
		
			return $str;
		}

		// render_callback : form
		public function render_form( $attributes, $content, $block ) {
			// No changes if attributes is empty
			if ( empty( $attributes ) || empty( $attributes['adminEmails'] ) ) {
				return $content;
			}

			$html = '';
			if ( ! empty( $attributes['redirectUrl'] ) ) {
				$html = '><input type="hidden" name="redirect_url" value="' . esc_attr( esc_url( $attributes['redirectUrl'] ) ) . '" />';
			}

			// Add required html
			if ( $html ) {
				$content = preg_replace(
					'/' . preg_quote( '>', '/' ) . '/',
					$html,
					$content,
					1
				);
			}

			//Submit Button HTML markup : change link to button tag
			$content = $this->str_last_replace(
				'<a', 
				'<button',
				$content
			); 

			$content = $this->str_last_replace(
				'</a>', 
				'</button>',
				$content
			); 

			// Enqueue block styles
			$this->enqueue_block_styles( $attributes['formStyle'] );
			return $content;
		}

		// Enqueue Block local styles in head
		public function enqueue_block_styles( $style, $priority = 10 ) {

			if ( empty( $style ) || ! function_exists( 'wp_strip_all_tags' ) ) {
				return;
			}

			$action_hook_name = 'wp_footer';
			if ( function_exists( 'wp_is_block_theme' ) && wp_is_block_theme() ) {
				$action_hook_name = 'wp_head';
			}
			add_action(
				$action_hook_name,
				static function () use ( $style ) {
					echo '<style>' . wp_strip_all_tags( $style ) . "</style>\n";
				},
				$priority
			);
		}

		// save form schema
		public function save_gutena_forms_schema( $post_id, $post, $update ) {

			if ( empty( $post_id ) || empty( $post ) || ! function_exists( 'parse_blocks' ) || ! in_array( $post->post_type, array( 'post', 'page' ), true ) || ! function_exists( 'wp_is_post_revision' ) || wp_is_post_revision( $post_id ) || ! has_block( 'gutena/forms', $post ) ) {
				return;
			}

			 // developer.wordpress.org/reference/functions/parse_blocks/
			$form_schema = $this->get_form_schema( parse_blocks( $post->post_content ) );
			if ( empty( $form_schema ) || ! is_array( $form_schema ) ) {
				return;
			}

			// Save gutena form ids in array gutena_form_ids
			if ( ! empty( $form_schema['form_ids'] ) ) {
				$gutena_form_ids = get_option( 'gutena_form_ids' );
				if ( ! empty( $gutena_form_ids ) && is_array( $gutena_form_ids ) ) {
					$gutena_form_ids = array_merge( $gutena_form_ids, $form_schema['form_ids'] );
				} else {
					$gutena_form_ids = $form_schema['form_ids'];
				}
				update_option(
					'gutena_form_ids',
					$this->sanitize_array( $gutena_form_ids )
				);
			}

			// Save gutena form schema in wp option
			if ( ! empty( $form_schema['form_schema'] ) && is_array( $form_schema['form_schema'] ) ) {
				foreach ( $form_schema['form_schema'] as  $formSchema ) {
					if ( ! empty( $formSchema['form_attrs']['formID'] ) ) {
						update_option(
							sanitize_key( $formSchema['form_attrs']['formID'] ),
							$this->sanitize_array( $formSchema )
						);
					}
				}
			}

		}

		// Get Form schema from block parsing
		private function get_form_schema( $blocks, $formID = 0 ) {
			if ( empty( $blocks ) || ! is_array( $blocks ) ) {
				return;
			}

			$form_schema = array();
			$form_ids    = array();
			$innerblocks = array();

			foreach ( $blocks as $block ) {

				if ( ! empty( $block['blockName'] ) && 'gutena/forms' === $block['blockName'] && ! empty( $block['attrs']['formID'] ) ) {
					$formID                               = $block['attrs']['formID'];
					$form_ids[]                           = $formID;
					$form_schema[ $formID ]['form_attrs'] = $block['attrs'];
				}

				if ( ! empty( $block['blockName'] ) && 'gutena/form-field' === $block['blockName'] && ! empty( $block['attrs']['nameAttr'] ) ) {
					$form_schema[ $formID ]['form_fields'][ $block['attrs']['nameAttr'] ] = array(
						'nameAttr'  => $block['attrs']['nameAttr'],
						'fieldName' => empty( $block['attrs']['fieldName'] ) ? '' : $block['attrs']['fieldName'],
						'fieldType' => empty( $block['attrs']['fieldType'] ) ? 'text' : $block['attrs']['fieldType'],
						'maxlength' => empty( $block['attrs']['maxlength'] ) ? '' : $block['attrs']['maxlength'],
					);
				}

				if ( ! empty( $block['innerBlocks'] ) && is_array( $block['innerBlocks'] ) ) {
					$innerblocks = $this->get_form_schema( $block['innerBlocks'], $formID );
					$form_schema = array_merge_recursive( $form_schema, $innerblocks['form_schema'] );
					$form_ids    = array_merge( $form_ids, $innerblocks['form_ids'] );
				}
			}

			return array(
				'form_ids'    => $form_ids,
				'form_schema' => $form_schema,
			);
		}

		// sanitize_array
		private function sanitize_array( $array ) {
			if ( ! empty( $array ) ) {
				foreach ( (array) $array as $key => $value ) {
					if ( is_array( $value ) ) {
						$array[ $key ] = $this->sanitize_array( $value );
					} else {
						$array[ $key ] = sanitize_text_field( $value );
					}
				}
			}
			return $array;
		}

		// Submit Gutena Forms
		public function submit_form() {
			check_ajax_referer( 'gutena_Forms', 'nonce' );

			if ( empty( $_POST['formid'] ) ) {
				wp_send_json(
					array(
						'status'  => 'error',
						'message' => __( 'Missing form identity', 'gutena-forms' ),
					)
				);
			}

			$formID     = sanitize_key( wp_unslash( $_POST['formid'] ) );
			$formSchema = get_option( $formID );

			if ( empty( $formSchema ) || empty( $formSchema['form_attrs'] ) || empty( $formSchema['form_fields'] ) ) {
				wp_send_json(
					array(
						'status'  => 'error',
						'message' => __( 'Missing form details', 'gutena-forms' ),
					)
				);
			}
			$blog_title  = get_bloginfo( 'name' );
			$admin_email = sanitize_email( get_option( 'admin_email' ) );

			// Email To
			$to = empty( $formSchema['form_attrs']['adminEmails'] ) ? $admin_email : $formSchema['form_attrs']['adminEmails'];

			if ( ! is_array( $to ) ) {
				$to = explode( ',', $to );
			}

			foreach ( $to as $key => $toEmail ) {
				$to[ $key ] = sanitize_email( wp_unslash( $toEmail ) );
			}

			// Email Subject
			$subject = sanitize_text_field( empty( $formSchema['form_attrs']['adminEmailSubject'] ) ? __( 'Form received', 'gutena-forms' ) . '- ' . $blog_title : $formSchema['form_attrs']['adminEmailSubject'] );

			//Form submit Data for filter
			$form_submit_data = array(
				'formName' => empty( $formSchema['form_attrs']['formName'] ) ? '': $formSchema['form_attrs']['formName'],
				'formID' => $formSchema['form_attrs']['formID']
			);

			$fieldSchema = $formSchema['form_fields'];
			$body        = '';

			foreach ( $_POST as $name_attr => $field_value ) {
				$name_attr   = sanitize_key( wp_unslash( $name_attr ) );
				$field_value = sanitize_textarea_field( wp_unslash( $field_value ) );
				if ( empty( $fieldSchema[ $name_attr ] ) ) {
					continue;
				}

				$field_name = sanitize_text_field( empty( $fieldSchema[ $name_attr ]['fieldName'] ) ? str_ireplace( '_', ' ', $name_attr ) : $fieldSchema[ $name_attr ]['fieldName'] );

				//Form submit Data for filter
				$form_submit_data['submit_data'][ $field_name ] = $field_value;

				$body .= '<p><strong>' . esc_html( $field_name ) . '</strong> <br />' . esc_html( $field_value ) . ' </p>';

			}

			//Email headers
			$headers = array(
				'Content-Type: text/html; charset=UTF-8',
				'From: ' . esc_html( $blog_title ) . ' <' . $admin_email . '>',
			);

			//Apply filter for admin email notification
			$body    = apply_filters( 'gutena_forms_submit_admin_notification', $body, $form_submit_data );

			$body    = wpautop( $body, true );
			$subject = esc_html( $subject );
			$res     = wp_mail( $to, $subject, $body, $headers );

			if ( $res ) {
				wp_send_json(
					array(
						'status'  => 'Success',
						'message' => __( 'success', 'gutena-forms' ),
					)
				);
			} else {
				wp_send_json(
					array(
						'status'  => 'error',
						'message' => __( 'Sorry! your form was not submitted properly, Please try again later.', 'gutena-forms' ),
						'details' => __( 'Failed to send email', 'gutena-forms' ),
					)
				);
			}

		}

	}

	Gutena_Forms::get_instance();
}
