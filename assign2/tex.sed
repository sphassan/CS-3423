s/\\/\\verb+\\+/g
s/%/\\%/g
s/\^/\\^/g
s/--/-\\hspace{.01cm}-/g
1 s/^/\\begin{center} {\\bf\n/
1 s/$/\n} \\end{center}/
1 s/^/\\documentstyle[11pt]{article}\n\\begin{document}\n/
2 s/^/\\begin{description}\n/
$ s/$/\n\\end{description}/
$ s/$/\n\\end{document}/
s/\(^[A-Z][A-Z]*\)/\\item[\1] \\hfill \\\\/
/^[ 	]*[+-]/s/$/ \\\\/
