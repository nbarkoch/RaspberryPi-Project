#pragma once
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <memory>
#include <string>
#include <stdexcept>


class S3Uploader
{
private:
	const char * ACCESS_KEY_ID = "";      // replace them with your access key
	const char * SECRET_KEY = "";         // replace them with your secret key
	const char * BUCKET = "";//"s3-cpp-sample-bucket";
	Aws::Auth::AWSCredentials credentials;
	/**
		* Check if file exists
		*
		* Note: If using C++17, can use std::filesystem::exists()
	*/
	bool file_exists(const std::string& name);

	/**
 * Put an object into an Amazon S3 bucket
 */
	bool put_s3_object(const Aws::String& s3_bucket_name,
		const Aws::String& s3_object_name,
		const std::string& file_name,
		const Aws::String& region = "");

public:
	S3Uploader();
	int uploadToS3(std::string fileName, std::string filePath);
	void operator()(std::string fileName, std::string filePath) { 
		uploadToS3(fileName, filePath); 
	}
};

