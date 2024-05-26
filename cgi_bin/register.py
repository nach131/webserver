import sys
from urllib.parse import parse_qs
from pymongo import MongoClient

def parse_query_string(query_string):
    # Parse the query string into a dictionary
    parsed_dict = {k: v[0] for k, v in parse_qs(query_string).items()}
    return parsed_dict

url = 'mongodb://root:klingon@192.168.1.20:27017'
client = MongoClient(url)
db_name = 'prueba'

def create_user(user_data):
    try:
        client = MongoClient(url)
        db = client[db_name]
        coleccion = db['users']
        result = coleccion.insert_one(user_data)
    finally:
        client.close()

    return result.inserted_id

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py datos")
    else:
        query_string = sys.argv[1]
        user_data = parse_query_string(query_string)
        
        # Remove confirmPassword from user_data to avoid storing it
        if 'confirmPassword' in user_data:
            del user_data['confirmPassword']

        user_id = create_user(user_data)
        print(f"User created with ID: {user_id}")

# "username=nacho&email=nacho.mota%40gmail.com&password=12345&confirmPassword=12345"