@echo off
@rem this script was created to clear the repository from all the wrongly
@rem generated release tags.

@rem remove all local tags
for /f "tokens=* delims=" %%a in ('git tag -l') do ( git tag -d %%a )

@rem fetch a reference to the remote ones
git fetch

@rem delete all remote tags
for /f "tokens=* delims=" %%a in ('git tag -l') do ( git push --delete origin %%a )
