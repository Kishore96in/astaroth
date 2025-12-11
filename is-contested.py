#!/usr/bin/env python3
import sys
import authors

allowed_authors = authors.list()
current_author = "N/A"
comment_out = True
delim = " "


for line in sys.stdin:

    author = line.rstrip()
    if author not in allowed_authors:
        print(f"Contested because of {author}")
        exit(0)

print("OK")
exit(1)
