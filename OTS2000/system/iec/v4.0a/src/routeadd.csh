
#!/usr/bin/csh
#routeadd.csh:*  add route to National Dispatch Center

route add 10.19.7.0 10.80.12.254 -netmask 255.255.255.0

# add route to Central China Dispatch Center

route add 10.40.3.0 10.80.12.254 -netmask 255.255.255.0
