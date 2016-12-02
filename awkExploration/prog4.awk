BEGIN{first = 0; second = 0; third = 0; fourth = 0; fifth = 0}
/Ascend-Data-Rate/{if ($3 >= 0 && $3 <= 14400){first++} else if ($3 >= 14401 && $3 <= 19200){second++} else if ($3 >= 19201 && $3 <= 28800){third++} else if ($3 >= 28801 && $3 <= 33600){fourth++} else {fifth++}}
END{print "0-14400\t\t" first "\n14401-19200\t" second "\n19201-28800\t" third "\n28801-33600\t" fourth "\nabove 33600\t" fifth}
