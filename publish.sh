#!/bin/sh
PROJECT=libnatspec
git push --tags $@ ssh://vitlav@natspec.git.sourceforge.net/gitroot/natspec/natspec
git push --tags $@ ssh://vitlav@natspec.git.sourceforge.net/gitroot/natspec/natspec
git push --tags $@ git.alt:packages/$PROJECT.git
git push $@ git.alt:packages/$PROJECT.git
