################################################
Test kernel memory lock capability on Qserv node
################################################

***************
Build container
***************

On a Linux workstation run:

.. code-block:: bash

   git clone https://github.com/lsst/qserv_testmlk.git
   docker build qserv_testmlk/container -t qserv/test_mlk
   docker push qserv/test_mlk

*****************************
Test memlock on Qserv cluster
*****************************

On target node, run:

.. code-block:: bash

   docker pull qserv/test_mlk
   docker run -it --rm --volume=/mnt/qserv:/qserv qserv/test_mlk

Output should be:

.. code-block::

   Checking memory lock feature for /qserv/data/mysql/LSST/Object_0.frm
   memory_lock: 20782 bytes (20.2K) locked
   memory_lock: kernel interface (/proc/10/status) returns: VmLck:       24 kB
   Checking memory lock feature for /qserv/data/mysql/LSST/Source_0.MYD
   memory_lock: 8920917625 bytes (8.3G) locked
   memory_lock: kernel interface (/proc/11/status) returns: VmLck:  8711836 kB




