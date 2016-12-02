BEGIN{acceptedswrinde = acceptedcais = acceptedq = rejectedswrinde = rejectedcais = rejectedq = canceledswrinde = canceledcais = canceledq = 0}
NR==1{starttime = $1 " " $2 " " $3}
{endtime = $1 " " $2 " " $3}
($4 == "+" || $4 == "j") && $5 == "swrinde"{acceptedswrinde++}
($4 == "+" || $4 == "j") && $5 == "news.cais.net"{acceptedcais++}
($4 == "+" || $4 == "j") && $5 == "?"{acceptedq++}
$4 == "-" && $5 == "swrinde"{rejectedswrinde++}
$4 == "-" && $5 == "news.cais.net"{rejectedcais++}
$4 == "-" && $5 == "?"{rejectedq++}
$4 == "c" && $5 == "swrinde"{canceledswrinde++}
$4 == "c" && $5 == "news.cais.net"{canceledcais++}
$4 == "c" && $5 == "?"{canceledq++; acceptedq++}
END{print "\t\t\tIncoming News Feed Summary\n\n"
print "\t\taccepted\trejected\tcanceled"
print "swrinde:\t"acceptedswrinde"\t\t"rejectedswrinde"\t\t"canceledswrinde
print "news.cais.net:\t"acceptedcais"\t\t"rejectedcais"\t\t"canceledcais
print "?:\t\t"acceptedq"\t\t"rejectedq"\t\t"canceledq
print "\nStart Time =",starttime"\tEnd Time =",endtime
}
