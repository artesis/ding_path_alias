api = 2
core = 7.x

; Changed to download as git repository due to failing when applying
; patch when version of git is lower than 1.7.5.4 and option working-copy
; is enabled
projects[pathauto][type] = module
projects[pathauto][subdir] = contrib
projects[pathauto][download][type] = git
projects[pathauto][download][url] = http://git.drupal.org/project/pathauto.git
projects[pathauto][download][tag] = 7.x-1.0-rc2
projects[pathauto][patch][] = http://drupal.org/files/1299460.patch

projects[transliteration][subdir] = contrib
projects[transliteration][version] = "3.1"

projects[token][subdir] = contrib
projects[token][version] = "1.4"

projects[strongarm][subdir] = contrib
projects[strongarm][version] = "2.0"
