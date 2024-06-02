import sys
import asyncio
from motor.motor_asyncio import AsyncIOMotorClient
from urllib.parse import parse_qs
from asyncio.exceptions import TimeoutError

# url = 'mongodb://root:tomate@192.168.39.136:27017'
url = 'mongodb://root:klingon@192.168.1.20:27017'
# client = AsyncIOMotorClient(url)
client = AsyncIOMotorClient(url, serverSelectionTimeoutMS=4000)  # Timeout de 2 segundos
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
 
    try:
        user_id = await asyncio.wait_for(create_user(user_data_complete), timeout=4)  # Espera máximo 4 segundos
    except TimeoutError:
        return 1
    
    if user_id is None:
        return 1
    else:
        return 0

if __name__ == "__main__":
    result = asyncio.run(main())
    if result is not None:
        print(result)


# "username=nacho&email=nacho.mota%40gmail.com&password=12345&confirmPassword=12345"
