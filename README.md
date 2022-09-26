# Ground Station Manager

Refer to the README.md file under the following directory - sdrpp/misc_modules/ground_station_mgr for software design details.


# Compile Software
From the main directory, run make:

	$> make
 


# Build Package
To create the debian package, after compiling the software, run the following command from the main directory:

	$> dpkg-buildpackage -b
 
 Note this requires the 'debhelper' package to be installed.
 
 The following file shall be created in the parent directory:
	
	gsmgr_1.0.0-alpha_amd64.deb


 
# Install package
To install the package:

	sudo apt install ./gsmgr_1.0.0-alpha_amd64.deb

The software will be installed under the following directory:

	/opt/hai/gsmgr



# Execution

To run the software, execute the following commands

	$> export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/hai/gsmgr/lib

	$> cd /opt/hai/gsmgr/bin
 
	$> ./sdrpp -r ../sdrpp
 
	$> ./predict -s -n 10099 -t /var/opt/hai/gsm/predict.tle

	$> ./rotctld --verbose --port=4533 --rot-file=/dev/ttyACM0
	

 
 
 