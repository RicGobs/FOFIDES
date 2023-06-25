# FOFISET Prototype - Website code

## Introduction
In this directory there is the code for the Website, this has to be put in the AWS Environment. <br/>
Here there is the tutorial of AWS: [Link Here](https://aws.amazon.com/it/getting-started/hands-on/build-serverless-web-app-lambda-apigateway-s3-dynamodb-cognito/).

Here there is all the architecture, we will focus from Iot-Core to the Website.

![net_arch](https://github.com/RicGobs/Fire-Alarm-System/blob/main/images/Network_Architecture.png) <br>

<br/>

1. Create an AWS Account.

<br/>

2. Create two different Roles:
- **IAM-ROLE-IOT**: this is needed to use IoT-Core properly and to create the IoT-Rules. It has to have the following policies: AmazonDynamoDBFullAccess, AWSIoTRuleActions, AWSIoTDataAccess and AWSIoTConfigAccess.

- **IAM-ROLE-LAMBDA**: this is needed to use AWS-Amplify, API-Gateway, DynamoDB and Lambda together. It has to have the following policies: AmazonDynamoDBFullAccess,IAMFullAccess, AWSLambdaBasicExecutionRole, AmazonAPIGatewayAdministrator, AmazonAPIGatewayInvokeFullAccess, AWSLambda_FullAccess and AdministratorAccess-Amplify.

<br/>

3. Create a DynamoDB (FireSensorTable in my case):
* When you create it, put Timestamp as Partition Key 
* select Number as type (not String)
* This table stores all the data that will arrive from the Thing (IoT Core)

<br/>

4. Create a IoT-Rule to insert the data, that are arriving from the Thing, in the DynamoDB. To create it:
* Go to the AWS IoT service in the AWS Management Console.
* Click on "Message Routing" in the left navigation pane and then select "Rules."
* Click on "Create" to create a new rule.
* Provide a name for your rule and add a description if desired.
* Under "Rule query statement," enter the SQL query that matches the data you want to send to your Lambda function. For example in my case: SELECT * FROM 'data/fire'.
* Under "Set one or more actions," click on "Add action" and select "Send a message to a Lambda function."
* Select your Lambda function from the drop-down menu.
* Click on "Create rule" to create your IoT rule.

<br/>

5. Create the lambda function to put items in the DynamoDB:
* Go to the AWS Management Console and navigate to the AWS Lambda service.
* Click on "Create function" and choose "Author from scratch."
* Provide a name for your function and select Python 3.9
* Under "Permissions," choose the IAM-ROLE-LAMBDA created before
* Click on "Create function" to create your Lambda function.
* Write the code for your Lambda function to process and insert the data into DynamoDB. The code is in the directory Code_Lambda_AWS and it is FireSensorTakeFunction. 

<br/>

6. Create another Lambda Function to take the data from the dynamoDB and use it for the website. <br/> Do the same process of the step before. But here, when you write the code for your Lambda function, use the other file. The code is in the directory Code_Lambda_AWS and it is FireSensorMonitorFunction. 

<br/>

7. Create the API, using API Gateway:
* Go to the AWS Management Console and navigate to the API Gateway service.
* Click on "Create API" and choose "REST API."
* Select "New API" and provide a name for your API.
* Under "Resources," click on "Actions" and select "Create Method."
* Choose "GET" as the method type and click on the checkmark button.
* Configure the integration of the GET method: Choose "Lambda Function" as the integration type, Select the appropriate region, Choose the Lambda function you created on the step before (FireSensorMonitorFunction), Click on the checkmark button to save the integration.
* Click on the "Method Request" card and scroll down to the "HTTP Request Headers" section.
* Click on the "Add header" button and click then enable CORS
* Click on the checkmark button to save the header.
* Click on the "Actions" dropdown button and select "Deploy API."
* Choose "New Stage" and provide a stage name. For example, dev.
* Click on deploy the API.

<br/>

8. Now you have created an API. Create the website and connect API Gateway and Amplify:
* Go to the AWS Amplify console for your project
* Create a new backend environment, click on New App and then on Host Web App
* Click on Deploy without git provider
* Create the index.html, index.js and index.css
* Compress all the files in a zip
* Upload it on the AWS Interface
* Click on the Amplify Link and you will be redirected on the website

<br/>

The Tutorial is conclused, now you have your website.