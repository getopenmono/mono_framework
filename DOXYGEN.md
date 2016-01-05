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

## Release documentation

Run
```
$ aws s3 cp generated/html/. s3://framework.kaleidoscope.one/ --recursive
```

(or upload the contents of `generated/` to the [proper AWS S3 bucket](https://console.aws.amazon.com/s3/home?region=us-east-1&bucket=framework.kaleidoscope.one)).
