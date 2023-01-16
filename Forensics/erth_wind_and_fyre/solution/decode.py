from PIL import Image
import numpy as np


def import_image(filename):
    '''
    loads an image and returns a numpy array of the image
    param filename (str): name of the image to be loaded

    output (ndarray): 3D array of the image pixels (RGB)
    '''
    return np.array(Image.open(filename))


def decode_capacity(img_copy):
    '''
    extract length of secret file from the image
    param img_copy (ndarray): a 1d vector of the image (flattened)
    output (int): the length of the secret file embedded to this image
    '''
    
    # get the 2 lsb from the first 12 pixels (24 bits)
    bin_rep = ''.join([bits_representation(pixel)[-2:] for pixel in img_copy[:12]])
    # return it as an integer
    return int(bin_rep, 2)

def bits_representation(integer, n_bits=8):
    '''
    takes an integer and return its binary representaation
    param integer (int): The integer to be converted to binary
    param n_bits (int): number of total bits to return. Default is 8

    output (str): string which represents the bits of the integer value

    Example: bits_representation(3, 8) >> 00000011
    '''
    return ''.join(['{0:0',str(n_bits),'b}']).format(integer)

def decode_secret(flat_medium, sec_ext, length, output_name):
    '''
    takes the image, length of hidden secret and the extension of the output file,
    then extracts secret file bits from the image and write it to a new file having
    the specified extension.
    param flat_medium (ndarray): a 1d vector of the image (flattened)
    param sec_ext (str): the file extension of the secret file. example: txt
    param length (int): the length of secret file extracted using decode_capacity
    '''
    # opening a file
    with open(''.join([f'{output_name}.',sec_ext]),'w', encoding="utf-8") as file:
        # extract 1 byte at a type (2 bits from each of the 4 pixels)
        for pix_idx in range(12,len(flat_medium[12:]),4):
            # convert the byte to character then write to file
            byte = ''.join([bits_representation(pixel)[-2:] for pixel in flat_medium[pix_idx:pix_idx+4]])
            file.write(chr(int(byte,2)))
            if pix_idx+4 == length:
                break


def decode(stego_img, sec_ext, output_name):
    '''
    this is the driver function to decode a secret file from the stego image
    param stego_img(str): name of the stego image to extract secret from
    param sec_ext(str): the extension of the secret file
    output: a secret file with the specified extension
    '''
    # read image and flatten to 1D
    img = import_image(stego_img).flatten()

    # decode secret length from stego image
    secret_size = decode_capacity(img)
    print(f'secret size: {secret_size}')

    # extract secret file from stego image
    decode_secret(img, sec_ext, secret_size, output_name)

    print(f'Decoding completed, "secret.{sec_ext}" should be in your directory')


# Decode all images in a set of folders

import os

for i in range(25):
    for filename in os.listdir(f'./images/{i}'):
        decode(f'./images/{i}/{filename}', 'txt', f'./output/{filename}')
