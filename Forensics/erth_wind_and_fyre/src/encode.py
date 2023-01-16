# Encodes python script into image

from PIL import Image
import numpy as np



def read_secret(filename):
    '''
    take a file name and return its contents
    param filename (string): the name of the file to read
    output (str): the content of the file
    '''
    with open(filename,'r') as f:
        b = f.read()
    return b

def import_image(filename):
    '''
    loads an image and returns a numpy array of the image
    param filename (str): name of the image to be loaded

    output (ndarray): 3D array of the image pixels (RGB)
    '''
    return np.array(Image.open(filename))

def reset(pixel, n_lsb):
    '''
    Takes an integer and set n least significant bits to be 0s
    param pixel (int): the set of bits to be modified. Ex: 255
    param n_lsb (int): number of least significant bits to set as 0s Ex: 2

    output (int): integer representing to byte after resetting n-lsb
    
    Example: reset(7,1) >> 6
    clarification: 0b111 >> 0b110
    '''
    return (pixel >> n_lsb) << n_lsb


def bits_representation(integer, n_bits=8):
    '''
    takes an integer and return its binary representaation
    param integer (int): The integer to be converted to binary
    param n_bits (int): number of total bits to return. Default is 8

    output (str): string which represents the bits of the integer value

    Example: bits_representation(3, 8) >> 00000011
    '''
    return ''.join(['{0:0',str(n_bits),'b}']).format(integer)

def find_capacity(img, code):
    '''
    Takes a 3D image and the secret file and return their size in 2-bit pairs
    param img (ndarray): the 3d array of the image to be used as a medium
    param code (str): the file you want to hide in the medium

    output medium_size(int): the available size to hide data (in 2 bit pair)
    output secret_size(int): the size of the secret file (in 2 bit pair)
    '''
    # total slots of 2 bits available after deducting 12 slots for size payload
    medium_size = img.size - 12
    # number of 2 bits slots the code needs
    secret_size = (len(code)*8) // 2

    print(f'Total Available space: {medium_size} 2-bit slots')
    print(f'Code size is: {secret_size} 2-bit slots')
    print('space consumed: {:.2f}%'.format((secret_size/medium_size) * 100))

    return medium_size, secret_size


def size_payload_gen(secret_size, n_bits_rep=8):
    '''
    Takes a binary representation and returns a pair of 2 bits untill finished
    param secret_size (int): an integer to be converted to binary representation
    param n_bits_rep (int): total number of bits. example 8 means there will be 8 bits in total

    output (str): two bits of the binary representation from the most significant bit to least significant
    '''
    # get the binary representation of secret size
    rep = bits_representation(secret_size,n_bits_rep)

    # return 2 bits at a time from msb to lsb
    for index in range(0, len(rep), 2):
        yield rep[index:index+2]

def secret_gen(secret, n_bits_rep=8):
    '''
    Takes the secret file and return 2 bits at a time until done
    param secret (str): the secret file
    param n_bits_rep (int): total number of bits. example 8 means there will be 8 bits for each character

    output (str): two bits of the binary representation of each character
    '''

    # for each character
    for byte in secret:

        # get its binary representation (8 bits)
        bin_rep = bits_representation(ord(byte),8)

        # return 2 bits at a time
        for index in range(0,len(bin_rep),2):
            yield bin_rep[index: index+2]


def encode_capacity(img_copy, sec_size):
    '''
    Encode the length of the secret file to the image (payload has a standard size of 24 bits)
    param img_copy (ndarray): a 1d vector of the image (flattened)
    param sec_size (int): the size of the secret as an integer
    '''
    
    # get the bits representation of the length(24 bits)
    g = size_payload_gen(sec_size, 24)

    # embed each 2-bit pair to a pixel at a time
    for index, two_bits in enumerate(g):
        
        # reset the least 2 segnificant bits
        img_copy[index] = reset(img_copy[index], 2)
        
        # embed 2 bits carrying info about secret length
        img_copy[index] += int(two_bits,2)


def encode_secret(img_copy, secret):
    '''
    Encode the secret file to the image
    param img_copy (ndarray): a 1d vector of the image (flattened)
    param secret (str): the secret file to be encoded into the image
    '''
    # generate 2 bits pair at a time for each byte in secret
    gen = secret_gen(secret)

    # embed to the image
    for index, two_bits in enumerate(gen):
        
        # +12 to prevent overlaping with the size payload bits
        img_copy[index+12] = reset(img_copy[index+12], 2)
        # embed 2 bits of secret data
        img_copy[index+12] += int(two_bits,2)



def encode(img_file, secret_file, output_name):
    '''
    this is the driver function to encode a secret file to an image
    param image_file(str): name of the image to be used as a medium to hide the secret
    param secret_file(str): name of the secret file
    output: a stego image at the same directory named as "stego image.png"
    clarification: all names must include extension. example: file.jpg
    '''
    #read the secret file
    secret = read_secret(secret_file)

    # read the image
    img = import_image(img_file)
    
    # find the size of secret file 
    medium_size, secret_size = find_capacity(img,secret)

    
    # if secret file is too large for the image or user wants to exit
    if secret_size >= medium_size:
        print('secret file is large for this image, please get a larger image')
        return None

    # save dimensions of image then flatten it
    img_dim = img.shape
    img = img.flatten()

    # encode length to image
    encode_capacity(img, secret_size)

    # encode secret file to image
    encode_secret(img, secret)

    # reshape the image back to 3D
    img = img.reshape(img_dim)
    
    # save
    im = Image.fromarray(img)
    im.save(output_name)
    print(f'Done, "{output_name}" should be in your directory')


# Encode to our images by folder (all images in the folder will have the same script encoded)

import os

for i in range(26):
    path = f'output/{i}/'
    files_list = os.listdir(path)
    for file in files_list:
        encode(os.path.join(path, file), f'scripts/{i}.py', f'output_image/{i}/{file}')
