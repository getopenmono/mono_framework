# Generating Static Documentaion

## Prerequisites

You need to install [Doxygen](http://www.stack.nl/~dimitri/doxygen/)
```
$ brew install doxygen
```
and the AWS command-line interface:
```
$ sudo pip install awscli
$ aws configure
```
and type in your IAM keys and region us-east-1.  You will need your AWS user
to be in a group that has AmazonS3FullAccess rights.

## Generating documentation

Run
```
$ doxygen
```
which places the generated HTML in the folder `generated`.

## Update documentation

If you want to update an existing version of the documentation, you can
synchronise the content with AWS S3 bucket:
```
$ aws s3 sync generated/html/. s3://framework-`date +%Y-%m-%d`/ --recursive
```

## Release new version of documentation

To create a new S3 bucket on AWS:
```
$ aws s3 mb s3://mono-framework-`date +%Y-%m-%d`
```

In the AWS Console, set the new bucket to "Enable website hosting", and
attach an access policy to the bucket so that everyone can view it
(insert the <date>):
```
{
  "Version": "2012-10-17",
  "Statement": [
    {
       "Effect": "Allow",
       "Principal": "*",
        "Action": ["s3:GetObject"],
        "Resource": ["arn:aws:s3:::mono-framework-<date>/*"]
    }
  ]
}
```
(The Version 2012-10-17 is a [fixed value](http://docs.aws.amazon.com/IAM/latest/UserGuide/reference_policies_variables.html).)

(For now we just use AWS Console to set the policy manually but it should
be possible to do it from the CLI).

Then upload the HTML:
```
$ aws s3 cp generated/html/. s3://mono-framework-`date +%Y-%m-%d`/ --recursive
```
And check that the resulting website (http://mono-framework-2016-01-08.s3-website-us-east-1.amazonaws.com/)
is what you expect.

Then make the default documentation site point to the new version by changing
the main S3 bucket (framework.openmono.com) to "Redirect all request to" the
S3 bucket you just created (mono-framework-<data>.s3-website-us-east-1.amazonaws.com).

TODO: To not expose the ugly AWS bucket URL to users, you would have to manually have to create
a new DNS CNAME record that points to a prettier URL and then create new buckets
with the pretier name.

## AWS initial setup

If you need to move to a new account, say.

- [Create an S3 bucket](http://docs.aws.amazon.com/cli/latest/userguide/using-s3-commands.html)
  with the intended name on the site, eg. framework.openmono.com
- Make the primary DNS point to the S3 bucket by creating a CNAME record
  that points framework.openmono.com to framework.openmono.com.s3-website-us-east-1.amazonaws.com

