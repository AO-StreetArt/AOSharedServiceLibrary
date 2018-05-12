We love pull requests from everyone.  By participating in this project, you agree to abide by the Code of Conduct, specified in the CODE_OF_CONDUCT.md file.

Start by forking and cloning the repository.  We provide a Vagrantfile to setup a development environment, but this requires that you install [Vagrant](https://www.vagrantup.com/).

Once you have Vagrant installed, cd into the main directory and run:

`vagrant up`

Once the box starts, you can enter it with:

`vagrant ssh`

The Project folder on your machine is synced to the /vagrant folder in the VM, so you will
need to move there before building.  Once in that folder, ensure that the tests pass:

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
