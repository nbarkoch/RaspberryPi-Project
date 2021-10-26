#include "S3Uploader.h"



S3Uploader::S3Uploader() {

	this->credentials.SetAWSAccessKeyId(Aws::String(ACCESS_KEY_ID));
	this->credentials.SetAWSSecretKey(Aws::String(SECRET_KEY));

}

bool S3Uploader::file_exists(const std::string & name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

bool S3Uploader::put_s3_object(const Aws::String & s3_bucket_name, const Aws::String & s3_object_name, const std::string & file_name, const Aws::String & region)
{
	// Verify file_name exists
	if (!file_exists(file_name)) {
		std::cout << "ERROR: NoSuchFile: The specified file does not exist"
			<< std::endl;
		return false;
	}

	// If region is specified, use it
	Aws::Client::ClientConfiguration clientConfig;
	if (!region.empty())
		clientConfig.region = region;
	Aws::S3::S3Client s3_client(this->credentials, clientConfig);

	// Set up request
	// snippet-start:[s3.cpp.put_object.code]

	Aws::S3::Model::PutObjectRequest object_request;
	object_request.SetBucket(s3_bucket_name);
	object_request.SetKey(s3_object_name);
	object_request.WithACL(Aws::S3::Model::ObjectCannedACL::public_read_write);
	const std::shared_ptr<Aws::IOStream> input_data =
		Aws::MakeShared<Aws::FStream>("SampleAllocationTag",
			file_name.c_str(),
			std::ios_base::in | std::ios_base::binary);
	object_request.SetBody(input_data);

	std::cout << "ready for putting object" << s3_object_name <<std::endl;

	// Put the object
	auto put_object_outcome = s3_client.PutObject(object_request);
	if (!put_object_outcome.IsSuccess()) {
		auto error = put_object_outcome.GetError();
		std::cout << "ERROR: " << error.GetExceptionName() << ": "
			<< error.GetMessage() << std::endl;
		return false;
	}
	return true;
}




int S3Uploader::uploadToS3(std::string fileName, std::string filePath)
{
	// stationID/cameraID/prefix(photo/video)ID.format

	Aws::SDKOptions options;
	Aws::InitAPI(options);
	{
		// Assign these values before running the program
		const Aws::String bucket_name = BUCKET;
		const Aws::String object_name = Aws::String(fileName);
		const std::string file_name = filePath;
		const Aws::String region = "us-east-2";      // Optional

		// Put the file into the S3 bucket
		if (put_s3_object(bucket_name, object_name, file_name, region)) {
			std::cout << "Put file " << file_name
				<< " to S3 bucket " << bucket_name
				<< " as object " << object_name << std::endl;
		}
	}
	Aws::ShutdownAPI(options);
	return 0;
}
