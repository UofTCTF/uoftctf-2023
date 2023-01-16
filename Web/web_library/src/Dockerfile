FROM node:16.15.0-alpine
WORKDIR /app
COPY package.json /app
RUN npm install
COPY . /app
CMD node super_obvious_main_file_name.js
EXPOSE 8080