# Run memory lock at CC-IN2P3

## Shmux:

# on ccqservbuild
git clone https://github.com/lsst/qserv_testmlk.git $HOME/src
# on ccqserv101
docker cp $HOME/src/qserv_testmlk/ qserv:/tmp
docker exec qserv /tmp/qserv_testmlk/run.sh

## Kubernetes

# on ccqserv100 (k8s master)
kubectl cp ./src/qserv_testmlk worker-1:/tmp/qserv_testmlk
kubectl exec worker-1 /tmp/qserv_testmlk/run.sh
