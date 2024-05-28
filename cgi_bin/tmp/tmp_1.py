import sys
import asyncio
from motor.motor_asyncio import AsyncIOMotorClient
from urllib.parse import parse_qs

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
        print("Usage: python script.py datos")
        return 1
    
    query_string = sys.argv[1]
    user_data = parse_query_string(query_string)
    
    # Eliminar 'confirmPassword' de user_data si existe
    user_data.pop('confirmPassword', None)
    username = user_data.get('username')

    user_data_complete = {
        'username': user_data.get('username'),
        'email': user_data.get('email'),
        'password': user_data.get('password')
    }

    user_id = await create_user(user_data_complete)
    if user_id is None:
        return 1
    
    
    
    return 0

if __name__ == "__main__":
    exit_code = asyncio.run(main())
    sys.exit(exit_code)
