# Basic C/C++ TCP Server

Two Main Paths: 

1. `/` - GET Method : returns a basic HTML. 
2. `/monkey` - GET Method : returns a webp image.
3. Any other Path or Method : returns a 404. 


### Next Steps for improving this small project : 

- [ ] Add sanitization for the URLS
- [ ] Containerize this.
- [ ] Attach to the systemctl process. 
- [ ] Add try-catch to catch the exceptions.
- [ ] Improve Error Logging.
- [ ] Grab the filename from the URL - Add security.


# How to Build : 

1. `cd networking-basics`
2. `rm -rf bin build .cache`
3. `mkdir build && cd build`
4. `cmake ..`
5. `cmake --build .`

### Run the program : 

1. Go the project folder
2. `./bin/main` 


####
Supports : 

- [ ] Linux
- [ ] Windows
- [ ] MacOS