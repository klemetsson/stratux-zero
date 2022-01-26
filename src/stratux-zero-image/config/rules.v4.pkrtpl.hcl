*filter

# Allows all loopback (lo0) traffic and drop all traffic to 127/8 that doesn't use lo0
-A INPUT -i lo -j ACCEPT
-A INPUT ! -i lo -d 127.0.0.0/8 -j REJECT

# Accepts all established inbound connections
-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

# Allow DHCP
-A INPUT -p udp --dport 67:68 --sport 67:68 -j ACCEPT

# Allow mDNS
-A INPUT   -s 224.0.0.0/4 -j ACCEPT
-A FORWARD -s 224.0.0.0/4 -d 224.0.0.0/4 -j ACCEPT
-A OUTPUT  -d 224.0.0.0/4 -j ACCEPT

# Allows all outbound traffic
-A OUTPUT -j ACCEPT

# Allow access to the web interface from local network
-A INPUT -p tcp --dport 80 -s ${network_cidr} -j ACCEPT

# Allows restricted SSH connections from local network
${enable_ssh != true ? "#": ""}-A INPUT -p tcp --dport 22 -s ${network_cidr} -m conntrack --ctstate NEW -m recent --set
${enable_ssh != true ? "#": ""}-A INPUT -p tcp --dport 22 -s ${network_cidr} -m conntrack --ctstate NEW -m recent --update --seconds 30 --hitcount 6 -j REJECT
${enable_ssh != true ? "#": ""}-A INPUT -p tcp --dport 22 -s ${network_cidr} -j ACCEPT

# Allow ping
-A INPUT -p icmp -j ACCEPT

# log iptables denied calls (access via 'dmesg' command)
-A INPUT -m limit --limit 5/min -j LOG --log-prefix "iptables denied: " --log-level 7

# Reject all other inbound - default deny unless explicitly allowed policy:
-A INPUT -j REJECT
-A FORWARD -j REJECT

COMMIT
