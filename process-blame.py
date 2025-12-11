#!/usr/bin/env python3
import sys
import authors
from datetime import datetime

allowed_authors = authors.list()
current_author = "N/A"
current_time = datetime.fromisoformat("1900-01-01")
comment_out = True
delim = " "

for line in sys.stdin:

    tokens = line.split(delim)
    if tokens[0] == "author":
        current_author = line[len(tokens[0]) + 1 :].rstrip()
    if tokens[0] == "author-time":
        current_time = datetime.fromtimestamp(int(line[len(tokens[0]) + 1 :].rstrip()))

    if current_author in allowed_authors:
        comment_out = False
    if current_author == "Oskar Lappi" and current_time > datetime.fromisoformat(
        "2021-04-30"
    ):
        comment_out = True

    if line[0] == "\t":
        if comment_out:
            sys.stdout.write(f"")
            # sys.stdout.write(f"// Copyright ({current_author}): {line[1:]}")
        else:
            sys.stdout.write(line[1:])
        comment_out = True
