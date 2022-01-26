*filter

# Allows all loopback (lo0) traffic and drop all traffic to ::1 that doesn't use lo0
-A INPUT -i lo -j ACCEPT
-A INPUT ! -i lo -d ::1 -j REJECT

# Accepts all established inbound connections
-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

# Allows all outbound traffic
# You could modify this to only allow certain traffic
-A OUTPUT -j ACCEPT

# Allow ping
-A INPUT -p icmp -j ACCEPT
-A INPUT -p icmpv6 -j ACCEPT

# log iptables denied calls (access via 'dmesg' command)
-A INPUT -m limit --limit 5/min -j LOG --log-prefix "iptables denied: " --log-level 7

# Reject all other inbound - default deny unless explicitly allowed policy:
-A INPUT -j REJECT
-A FORWARD -j REJECT

COMMIT
