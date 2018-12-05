
# build the app
docker build -t friendlyhello .

# show available images
docker image ls

# run container in foreground
docker run -p 4000:80 friendlyhello

# run container detached
docker run -d -p 4000:80 friendlyhello

# show inactive containers
docker container ls --all

# start inactive container
docker container start 4efbb924c801

# stop container
docker container stop 4efbb924c801

# remove container
docker container rm 4efbb924c801