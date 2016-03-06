git init
git commit -m"[empty] Initial commit" --allow-empty
git tag initial <first-sha-1> -m"Tag initial commit"

git tag -l

alias git-ls='git log --decorate --oneline'
alias git-ll='git log --decorate --numstat'

# roll commit into previous commit
git commit --amend -C HEAD

