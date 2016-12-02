BEGIN{lscs2413=lscs2413d=runner2413=runner2413d=ringer2413=ringer2413d=rings2413=rings2413d=lscsarticles=lscsgroups=runnerarticles=runnergroups=ringeraticles=ringergroups=ringsarticles=ringsgroups=lscstimes=runnertimes=ringertimes=ringstimes=0}
NR==1{starttime=$1 " " $2 " " $3}
{endtime=$1 " " $2 " " $3}
$6 ~ /lonestar/ && $7 == "group" && $8 == "utsa.cs.2413"{lscs2413+=$9}
$6 ~ /lonestar/ && $7 == "group" && $8 == "utsa.cs.2413.d"{lscs2413d+=$9}
$6 ~ /runner/ && $7 == "group" && $8 == "utsa.cs.2413"{runner2413+=$9}
$6 ~ /runner/ && $7 == "group" && $8 == "utsa.cs.2413.d"{runner2413d+=$9}
$6 ~ /ringer/ && $7 == "group" && $8 == "utsa.cs.2413"{ringer2413+=$9}
$6 ~ /ringer/ && $7 == "group" && $8 == "utsa.cs.2413.d"{ringer2413d+=$9}
$6 ~ /ring[0-9][0-9]/ && $7 == "group" && $8 == "utsa.cs.2413"{rings2413+=$9}
$6 ~ /ring[0-9][0-9]/ && $7 == "group" && $8 == "utsa.cs.2413.d"{rings2413d+=$9}
$6 ~ /lonestar/ && $7 == "exit"{lscsarticles+=$9; lscsgroups+=$11}
$6 ~ /runner/ && $7 == "exit"{runnerarticles+=$9; runnergroups+=$11}
$6 ~ /ringer/ && $7 == "exit"{ringerarticles+=$9; ringergroups+=$11}
$6 ~ /ring[0-9][0-9]/ && $7 == "exit"{ringsarticles+=9; ringsgroups+=$11}
$6 ~ /lonestar/ && $7 == "times"{lscstimes+=$NF}
$6 ~ /runner/ && $7 == "times"{runnertimes+=$NF}
$6 ~ /ringer/ && $7 == "times"{ringertimes+=$NF}
$6 ~ /ring[0-9][0-9]/ && $7 == "times"{ringstimes+=$NF}
END{print "\t\t\tNews Reader Summary\n\n"
print "\t\tlonestar\trunner\t\tringer\t\trings\n"
print "Articles:\t"lscsarticles"\t\t"runnerarticles"\t\t"ringerarticles"\t\t"ringsarticles
print "Groups:\t\t"lscsgroups"\t\t"runnergroups"\t\t"ringergroups"\t\t"ringsgroups
print "Cs2413:\t\t"lscs2413"\t\t"runner2413"\t\t"ringer2413"\t\t"rings2413
print "Cs2413.d:\t"lscs2413d"\t\t"runner2413d"\t\t"ringer2413d"\t\t"rings2413d
print "User Time:\t"lscstimes"\t\t"runnertimes"\t\t"ringertimes"\t\t"ringstimes
print "Start Time =",starttime"\t\tEnd Time =",endtime
}
