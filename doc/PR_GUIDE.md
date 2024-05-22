# Pull Request Guide

## Set up local machine

### Fork pgmoneta into your GitHub account

1. Go to [pgmoneta_ext](https://github.com/pgmoneta/pgmoneta_ext)
2. Create a fork by cliking at fork button on the right top corner
3. A forked repository will be created in your account.

### Clone on your local machine

Navigate to the directory where you wish to place the source code. In this example, we'll use `$HOME/` as the base directory.

``` sh
cd $HOME/
git clone https://github.com/YOUR_USERNAME/pgmoneta_ext pgmoneta_ext
cd pgmoneta_ext
git remote add upstream https://github.com/pgmoneta/pgmoneta_ext
git fetch --all
```

Now you can check your remote using

``` sh
git remote -v
```

One is from your GitHub account repository, and the other is from the `pgmoneta_ext` repository

## Commit

If you want to make some changes, it's better to create a new branch.

``` sh
git checkout main
git pull
git branch
git checkout -b new-branch main
git branch
```

After this, you will switch to the new-branch branch, where you can manipulate the code and make some changes.

You can also delete a branch other than the current one using the following command:

``` sh
git branch -D branch-name
```

### Submit commit

Before committing, you need to stage the changes you want to include in the commit. You can stage changes to specific files or all changes in the working directory.

- To stage a specific file:

    ``` sh
    git add filename
    ```

- To stage all changes:

    ``` sh
    git add .
    ```

Once the changes are staged, you can commit them to your local repository.

``` sh
git commit -m "Your commit message"
```

You can check the commit using:

``` sh
git log
```

### Delete commit

1. Remove the last commits from git:

    ``` sh
    git reset --hard <the one you revert to, using the commit hash value>
    ```

2. Remove a specific one commit:

    - First, get commits (Here, we retrieve the last 4 commits)

        ``` sh
        git rebase -i HEAD~4
        ```

    - If you encounter an error stating `error: unable to start editor 'emacs'`, you can use the command below to change the editor.

        ``` sh
        git config --global core.editor "vim"
        ```

3. You will see

    ``` console
    pick be5b2e1 4-th commit
    pick 3d50463 3-rd commit
    pick 93d3de2 2-nd commit
    pick 347a2c5 1-st commit
    ```

4. Change the line you want to delete by changing `pick` to `drop`, then quit and save.

### Squash commit

1. Start an interactive rebase:

    - Determine how many commits you want to combine.

        ``` sh
        git rebase -i HEAD~3
        ```

2. Choose commits to combine:

    - You wil see

        ``` console
        pick 3d50463 3-rd commit
        pick 93d3de2 2-nd commit
        pick 347a2c5 1-st commit
        ```

    - To combine these commits, you'll need to change the word pick to squash. The first commit will be the base for the squash.

3. Rewrite commit message:

    - After you save and close the file, another editor window will open, allowing you to rewrite the commit message for the new, combined commit.

4. Complete the Rebase:

    - Once you save and close the commit message file, Git will complete the rebase process and squash the commits into one.

## Push

Nomally we just use

``` sh
git push origin branch_name
```

After squashing commits or rewriting history, we have to use force push

``` sh
git push --force origin branch_name
```

## Rebase

When working with a forked repository, it's common to keep your fork up to date with the upstream repository. Before we submit pull request, we should rebase our forked repository with the upstream main branch.

``` sh
git fetch upstream       # fetch the latest changes
git checkout main        # switch to the repository you want to rebase.
git rebase upstream      # rebase the current branch with the upstream repository
```

## Pull request

Now you can use your GitHub account to submit a pull request to the community.
