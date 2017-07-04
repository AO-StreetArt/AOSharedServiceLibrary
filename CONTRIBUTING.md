We love pull requests from everyone.  By participating in this project, you agree to abide by the Code of Conduct, specified in the CODE_OF_CONDUCT.md file.

Start by forking and cloning the repository, then following the instructions to [build from source](http://aosharedservicelibrary.readthedocs.io/en/latest/quickstart.html).

Note that you may, at your leisure, build and test your changes within the [AOSSL Docker Image](http://aosharedservicelibrary.readthedocs.io/en/latest/quickstart.html).  Simply run the image, clone your fork inside of it, and issue the below instructions from within the container.  This has the added benefit of coming with all of the dependencies pre-installed.

Ensure that the tests pass:

`make && make test`

Make your changes and write tests for your changes, then ensure that all tests pass:

`make clean && make && make test`

Push to your fork and submit a pull request.

At this point you're waiting on us. We like to at least comment on pull requests within three business days (and, typically, one business day). We may suggest some changes or improvements or alternatives.

Some things that will increase the chance that your pull request is accepted:

* Write tests
* Comment your code
* Write a good commit message
* If you have many different commits, use rebase to consolidate down to a single commit
* Use [CppLint](https://github.com/google/styleguide/tree/gh-pages/cpplint) and follow the [Google C++ Style Guide](https://github.com/google/styleguide) wherever possible
