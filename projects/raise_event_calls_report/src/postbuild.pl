#!/usr/bin/perl -w

use strict;
use Cwd;

unshift(@INC, $ENV{DPO2_CORE_ROOT} . "/scripts");
require DPOPostbuild;


my $root_dir = $ENV{"RAISE_EVENT_CALLS_REPORT_PRJ_ROOT"};
$root_dir =~ s/\\/\//g;

# read version of the project
my $major=0;
my $minor=0;
my $patch=0;
if (!DPOUtils::read_project_version($root_dir,
                                    \$major,
                                    \$minor,
                                    \$patch))
{
    print "Can not extract project version from $root_dir.\n";
    exit -1;
}

print "(postbuild.pl) version = $major.$minor.$patch\n";

if (!DPOPostbuild::dpo_postbuild("raise_event_calls_report"))
{
    exit -1;
}

