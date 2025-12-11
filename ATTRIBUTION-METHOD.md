# Methods used to remove copyrighted intellectual property not attributable to Aalto University


## Contributors

The filtered code in this branch is based on the public develop branch 313fbc8984310e08294750cceb14b4508c114c99, committed on Tue Dec 9 16:07:38 2025 +0200.

The authors employed by the Aalto University during the development of the code, by Git identifier:
"jpekkila",
"Johannes Pekkilä",
"Pekkilä Johannes",
"Johannes Pekkila",
"Matthias Rheinhardt",
"mrheinhardt",
"Maarit Korpi-Lagg",
"Touko Puro",
"Puro Touko",
"purotouko",
"ToxPuro",
"toxpuro",
"Frederick Gent",
"fredagent",
"fredgent",
"Oskar Lappi", # Until 2021-04-31

We denote these contributors as "Aalto Authors". The work produced by Aalto Authors is attributed to Aalto University.

All authors who have contributed to Astaroth, by the number of commits on c29c497d73b001cb439c0bb7f6966f5358f1f14d:
```bash
git shortlog -s -n
  3670  jpekkila
   910  ToxPuro
   441  Miikka Vaisala
   330  purotouko
   315  Touko Puro
   150  Oskar Lappi
   112  Matthias Rheinhardt
    99  Puro Touko
    49  JackHsu
    39  mrheinhardt
    31  Miikka Väisälä
    23  Maarit Korpi-Lagg
    22  Pekkilä Johannes
    15  Johannes Pekkila
    15  toxpuro
     5  Johannes Pekkilä
     5  fredagent
     4  Frederick Gent
     2  Indrani Das
     2  Joern Warnecke
     2  Maarit Käpylä
     2  fredgent
     1  Tzu-Chun Hsu
```

We denote all contributors not part of "Aalto Authors" as "Other Authors"



## Method

The files have been filtered as follows.
1. Clearly contested directories and files were removed without further analysis (certain samples, configurations, tests, 3rd party code, etc)
2. Files containing any contributions from Other Authors except merge commits at any time in the history of the repository were identified and removed from the repository.
3. All lines of codes with authors attributed to non-aalto authors were removed
4. We did a manual inspection of the remaining files and removed those that we were unsure originated from Aalto Authors

### Justification
Merge commits were excluded, because by good convention, they should not contain new code and the commit history should be public to determine proper attribution. Furthermore, merges typically result in superficial changes (formatting, whitespaces), which can result in false attribution.


### Details

For more details on the approach, see AUDIT.md