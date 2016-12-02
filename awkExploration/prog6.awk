/^[^ 	]/{current = ""}
/User-Name/{current = $3; acctstop = 1}
/Acct-Status-Type/{if ($3 == "Stop"){acctstop = 0; sessions[current]++}}
acctstop == 0 && /Acct-Session-Time/{time[current] += $3}
acctstop == 0 && /Acct-Input-Packets/{inpackets[current] += $3}
acctstop == 0 && /Acct-Output-Packets/{outpackets[current] += $3}
END{for (names in sessions){
print("User:                  ", names) 
print("Number of Sessions:    ", sessions[names])
print("Total Connect Time:    ", time[names])
print("Input Bandwidth Usage: ", inpackets[names])
print("Output Bandwidth Usage:", outpackets[names])
print
}}
