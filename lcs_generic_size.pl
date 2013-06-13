#!/usr/bin/perl
# Author: Amoghavarsha Suresh

use strict;
use warnings;

sub rtrim($)
{
	my $string = shift;
	$string =~ s/\s+$//;
	return $string;
}

use Getopt::Long;
my $file1_name;
my $file2_name;
my $max_length;
my $min_length=10;
my $exe;
my $error_rate;
my $results_file="lcs_results.txt";

GetOptions("file1=s" =>\$file1_name,
		"file2=s"=>\$file2_name,
		"max_length=i"=>\$max_length,
		"min_length=i" =>\$min_length,
		"exe=s" => \$exe,
		"error_rate_max=i"=> \$error_rate,
		"results_file=s"=> \$results_file);

if( (! defined($file1_name)) || (!defined($file2_name)) || (!defined($max_length)) || (!defined($exe)) )
{
	die("\n\t Required options:  2 file with strings to compare , executable and maximum-length of the strings to consider. One or more of them is not available, please provide it. \n\n");
}

my $file1_handle;
my $file2_handle;
my $results_file_handle;
open( $file1_handle,"<","$file1_name")or die("\n\t Cannot open the file $file1_handle\n\t Exitting now! \n");

open( $file2_handle,"<","$file2_name")or die("\n\t Cannot open the file $file2_handle\n\t Exitting now! \n");

open( $results_file_handle,">","$results_file") or die("\n\t Cannot open the file $results_file\n\t Exitting now! \n");

print "\n\t Files read!! \n";
my @string1_array=<$file1_handle>;
my @string2_array=<$file2_handle>;

my $length;

my $exe_1="./".$exe;

`mkdir -p working_directory`;

my $lcs_output_file="./working_directory/lcs_output.txt";
my $lcs_noerror_output_file="./working_directory/lcs_noerror_output.txt";

my @lcs_output_contents; # Would contain output of a run of "exe"
my @lcs_found;# Working variable for searching instances of "LCS-result" or result rather.
my @result_length_found;

for($length=$min_length; $length<=$max_length;$length*=10)
{

	`$exe_1 $file1_name $file2_name $length 0 > $lcs_noerror_output_file`;	

	@lcs_output_contents=`cat $lcs_noerror_output_file`;
	
	@lcs_found=grep(/LCS\-result/,@lcs_output_contents);

	@result_length_found=grep(/Length\s*of\s*result\:/,@lcs_output_contents);

	print $results_file_handle "\n\n ------------------------------------------- \n";
	
	$lcs_found[0]=rtrim($lcs_found[0]);
	$result_length_found[0]=rtrim($result_length_found[0]);
	print $results_file_handle "\n\t Length: $length \n\t Error-free result: $lcs_found[0]  $result_length_found[0]";

	for(my $i=1;$i<=$error_rate;$i++)
	{	

		print "\n\t Starting mutltiple-runs for  length: $length and error-rate: $i";
		
		print $results_file_handle "\n\n \t Error-rate: $i \n"	;
		
		for(my $j=0; $j<10; $j++)
		{		
			
			`$exe_1 $file1_name $file2_name $length $i > $lcs_output_file`;

			@lcs_output_contents=`cat $lcs_output_file`;
	
			@lcs_found=grep(/LCS\-result/,@lcs_output_contents);
		
			@result_length_found=grep(/Length\s*of\s*result\:/,@lcs_output_contents);
		
			$lcs_found[0]=rtrim($lcs_found[0]);
			print $results_file_handle "\n\t Iteration: $j \t $lcs_found[0]  $result_length_found[0]";
		}
	
	}
print "\n";

		
}
print "\n";

