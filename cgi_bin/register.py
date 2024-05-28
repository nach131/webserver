import sys
# from pymongo import MongoClient
import asyncio
from motor.motor_asyncio import AsyncIOMotorClient
from urllib.parse import parse_qs

# url = 'mongodb://root:tomate@192.168.39.138:27017'
url = 'mongodb://root:klingon@192.168.1.20:27017'
client = AsyncIOMotorClient(url)
db_name = 'prueba'

def parse_query_string(query_string):
    parsed_dict = parse_qs(query_string)
    return {k: v[0] for k, v in parsed_dict.items()}

async def create_user(user_data):
    try:
        db = client[db_name]
        coleccion = db['users']
        result = await coleccion.insert_one(user_data)
        return result.inserted_id
    except Exception as e:
        print("An error occurred:", e)
        return None


async def main():
    if len(sys.argv) != 2:
     return print("Usage: python script.py datos")
    
    query_string = sys.argv[1]
    user_data = parse_query_string(query_string)
    
    user_data_complete = {
        'username': user_data.get('username'),
        'email': user_data.get('email'),
        'password': user_data.get('password')
    }

    # print(user_data_complete)
    
    user_id = await create_user(user_data_complete)
    # while user_id is None:
    #     # Si la inserción no se ha completado, esperar un momento antes de intentarlo de nuevo
    #     user_id = create_user(user_data_complete)
    if user_id is None:
        print("1")
    else:
        print("0")
        # print(user_id)
 

if __name__ == "__main__":
    result = asyncio.run(main())
    if result is not None:
        print(result)


# "username=nacho&email=nacho.mota%40gmail.com&password=12345&confirmPassword=12345"
