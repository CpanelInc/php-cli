
In order to build this you need to have libyaml installed on the system.

http://pyyaml.org/wiki/LibYAML

I have it linked statically, because I do not know how we want to deploy this.
Where the rpm installs both php_cli and libyaml.

Also I have it fully linked static, I need to spend time to make it static
only for libyaml.

NOTE: if Cpanel::ConfigFiles::Apache changes the location for paths.conf we need to
change this php_cli.c apache_conf_dir variable.

TODO when this is put into the main repo: Make comment in Cpanel::ConfigFiles::Apache
that if its logic changes to be sure to update php_cli.c accordingly also.

