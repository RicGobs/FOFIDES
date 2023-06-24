import json

# import the AWS SDK
import boto3

from decimal import Decimal

# Funtion to sort by numeric timestamp
def sort_by_key(list):
    return list['Timestamp']

# Define the handler function that the Lambda service will use
def lambda_handler(event, context):

    # Create a DynamoDB object using the AWS SDK
    dynamodb = boto3.resource('dynamodb')

    # Use the DynamoDB object to select our tables
    table_fire = dynamodb.Table('FireSensorTable')

    # Retrieve tuples of our tables to return
    response_fire = table_fire.scan()

    output_fire = response_fire['Items']

    # Convert numeric values to strings
    for fire_json in output_fire:
        fire_json['Timestamp'] = str(fire_json['Timestamp'])

    # Sort by timestamp
    output_fire = sorted(output_fire, key=sort_by_key)


    return {
       'statusCode': 200,
       'body_fire': json.dumps(output_fire)
    }
