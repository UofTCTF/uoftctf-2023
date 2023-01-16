import os
import shutil

# Step 1: Split into 25 folders

# create 25 folders
for i in range(26):
    os.mkdir('output/' + str(i))

# move images to folders
for i in range(1167):
    shutil.move('output/' + str(i) + '.png', 'output/' + str(i // 45))


# Step 2: Go through each character in the string "uoft{d0_y0u_r3m83r_53m83r}" and create a simple python script that prints the character. Save the scripts in the folder /scripts.

s = "uoft{d0_y0u_r3m83r_53m83r}"

for i in range(len(s)):
    with open('scripts/' + str(i) + '.py', 'w') as f:
        f.write('print("' + s[i] + '")')

# Step 3 is in encode.py

# Step 4: For every folder in /output_images, move all images in the folders named 0 to 26 to the parent folder /output_images.

for i in range(26):
    files_list = os.listdir('output_image/' + str(i))
    for file in files_list:
        shutil.move('output_image/' + str(i) + '/' + file, 'output_image/' + file)


# Step 5: Stitch together all images in output_image into a video using ffmpeg. The sound file is named source.mp3 in the working directory. Output is 30 fps.

# ffmpeg -r 29.97 -i steg_image/%d.png -i source.mp3 -vcodec png video.mov -y