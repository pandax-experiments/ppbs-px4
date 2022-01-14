#!/usr/bin/env perl

#   Copyright 2021 Xun Chen (chenxun@sjtu.edu.cn)
#
#   This script is part of ppbs.
#
#   ppbs is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   ppbs is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with ppbs.  If not, see <http://www.gnu.org/licenses/>.

use strict;
use File::Basename;
use feature 'say';
# usage
# ppbs-gen.pl input_header_file input_type_file

die "Usage: $0 input_header_file module_suffix input_type_file [index0,[index1...]]\n" unless scalar @ARGV==3 or scalar @ARGV==4;

sub parse_input {
    my ($fin) = @_;
  my $struct_start = 0;
  my $structs;
  my $current_name = '';
  while(<$fin>) {
    chomp;
    next if (m/^\s*\/\//);
    if (!$struct_start) {
      if ( m/(tuple|struct)\s+([\w\d]+)\s+\{/) {
        push @{$structs}, {'name' => $2, 'type' => $1};
        $current_name = $2;
        $struct_start = 1;
      } elsif ( m/enum\s+([\w\d]+)(\s+:?\s+([\w\d]+))?\s+\{/ ){
        $struct_start = 1;
        my $type = "uint32";
        $type = $3 if defined $3;
        push @{$structs}, {'name' => $1, 'type' => "enum : ".$type};
        $current_name = $1;
      }
      next;
    } elsif (m/\};/) {
      $struct_start = 0;
    } elsif (m/([\w\d]+)\s*=\s*(\d+);/) {
      my $name = $1;
      my $tag = $2;
      m/^\s*(.*)\s+$name/;
      my $type = $1;
      push @{$structs->[$#{$structs}]->{'var'}}, {'name' => $name,
                                                  'tag' => $tag,
                                                  'type' => $type};
    } elsif (m/([\w\d]+);/) {
      my $name = $1;
      m/^\s*(.*)\s+$name/;
      my $type = $1;
      push @{$structs->[$#{$structs}]->{'var'}}, {'name' => $name,
                                                  'type' => $type};
    } elsif (m/^\s*(.*\,.*)$/){
      push @{$structs->[$#{$structs}]->{'var'}}, {'name' => $1};
    }
  }
  return $structs;
}

# read the realm and top level information from header file
open(my $fh, "<", $ARGV[1]) or die "Cannot open file $ARGV[1]: $!\n";

my $realm = "";
my @types;

read $fh, my $file_content, -s $fh;
$file_content =~ s/\n/ /g;
$file_content =~ s/\s*(\(|\))\s*/$1/g;

$file_content =~ m/PBSF_DECLARE_REALM\((\w+)/;
$realm = $1;
#say $realm;

while($file_content =~ m/PBSF_REGISTER_TYPE\((\d+),\s*(\w+)/g) {
    my $type_code = $1;
    my $type = $2;
    if ($type_code < 100) {
	push @types, $type;
    }
}
close $fh or die "Cannot close file $ARGV[0]: $!\n";

# open the bsi file to read types

open(my $fh1, "<", $ARGV[2]) or die "Cannot open file $ARGV[2]: $!\n";
my $type_entries = parse_input ($fh1);

my @indices;
if (scalar @ARGV == 4) {
    @indices = split /,/, $ARGV[3];
}

my @sequential_readers;
my @indexed_readers;
sub gen_file_readers {
    for my $t (@types) {
	say "using ${t}Reader = SequentialReader<${t}, ${realm}>;";
	push @sequential_readers, "${t}Reader";
	for my $index (@indices) {
	    my $nidx = ucfirst $index;
	    my $idx = $index;
	    $idx =~ s/(int\d+)$/$1_t/g;
	    say "using ${t}${nidx}Reader = IndexedReader<${idx}, ${t}, ${realm}>;";
	    push @indexed_readers, "${t}${nidx}Reader";
	}
	print "\n";
    }
}

my $header_name = basename($ARGV[1]);
# generate code (to STDOUT)
print <<"EOT";
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <memory>
#include <string>

#include <bs3/pbs.hh>

#include "ppbs.hh"

#include "${header_name}"

namespace py = pybind11;
namespace pf = pbsf;

EOT

gen_file_readers();
gen_python_modules();

sub gen_python_modules {
    gen_python_modules_header();
    gen_python_readers();
    gen_python_type_binding();
    say "}";
}

sub gen_python_modules_header {
    my $mod_name = "ppbs_".$ARGV[0];
    print <<"EOT";
PYBIND11_MODULE(${mod_name}, m) {
    m.doc() = "pbs data reader";
    using namespace pybind11::literals;

EOT
}

sub gen_python_readers {
    for my $reader (@sequential_readers) {
	say "    declare_reader<${reader}>(m, \"${reader}\");";
    }
    for my $reader (@indexed_readers) {
	say "    declare_indexed_reader<${reader}>(m, \"${reader}\");";
    }
    print "\n";
}

sub gen_python_type_binding {
    for my $entry (@$type_entries) {
	my $name = $entry->{name};
	my $type = $entry->{type};
	if ($type eq "struct" || $type eq "tuple") {
	    say "    py::class_<${name}>(m, \"${name}\")\n        .def(py::init<>())";
	    if ($type eq "tuple" && grep (/^$name$/, @indices)) {
		# for names as index, add another constructor
		my $input = "";
		for my $e (@{$entry->{var}}) {
		    my $et = $e->{type};
		    $et =~ s/(int\d+)$/$1_t/g;
		    if (\$e == \@{$entry->{var}}[-1]) {
			$input = $input.$et;
		    } else {
			$input = $input.$et.", ";
		    }
		}
		say "        .def(py::init<".$input.">())";
	    }
	    gen_python_class_member($name, $entry->{var});
	} elsif ($type =~/enum/) {
	    say "    py::enum_<${name}>(m, \"${name}\")";
	    gen_python_enum_type($name, $entry->{var});
	}
    }
}

sub gen_python_class_member {
    my ($class, $var) = @_;
    for my $e (@$var) {
	my $n = $e->{name};
	if (\$e == \@$var[-1]) {
	    say "        .def_readonly(\"${n}\", &${class}::${n});\n";
	} else {
	    say "        .def_readonly(\"${n}\", &${class}::${n})";
	}
    }
}

sub gen_python_enum_type {
    my ($class, $var) = @_;
    for my $e (@$var) {
	my $n = $e->{name};
	my @enums = split /\s*,\s*/, $n;
	for my $enum (@enums) {
	    $enum =~ s/=.*//;
	    say "        .value(\"${enum}\", ${class}::${enum})";
	}
    }
    say "        .export_values();\n";
}
