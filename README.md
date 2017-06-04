# Nscan

Silently scan a network to find all active nodes.

To install clone the repository:

    git clone https://github.com/Dylan-halls/Nscan/

then run the installer

    cd Nscan
    chmod +x configure
    ./configure
    
once installed to do a quick scan of your network run

    $ nscan 192.168.*.* --subnet 1

this will scan every address in the subnet 192.168.1.0/24

you can scan ranges of subnets with 

    $ nscan 192.168.*.* --subnet-range 5-8

this will then scan all of 192.168.5.0/24 to 192.168.8.0/24

and for more arguments and help run

    $ nscan --help
