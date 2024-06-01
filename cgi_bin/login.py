import sys
import asyncio
from motor.motor_asyncio import AsyncIOMotorClient
from urllib.parse import parse_qs
from asyncio.exceptions import TimeoutError

# url = 'mongodb://root:tomate@192.168.39.136:27017'
url = 'mongodb://root:klingon@192.168.1.20:27017'
client = AsyncIOMotorClient(url, serverSelectionTimeoutMS=4000)  # Timeout de 4 segundos
db_name = 'prueba'

def parse_query_string(query_string):
    parsed_dict = parse_qs(query_string)
    return {k: v[0] for k, v in parsed_dict.items()}

async def verify_user(email, password):
    try:
          db = client[db_name]
          coleccion = db['users']
          user = await coleccion.find_one({"email": email})
        #   print(user)
          if user and user.get("password") == password:
               return True
          return False
    except Exception as e:
        print("An error occurred:", e)
        return None

async def main():
    query_string = sys.argv[1]
    user_data = parse_query_string(query_string)
    
    email = user_data.get('email')
    password = user_data.get('password')

    try:
        result = await asyncio.wait_for(verify_user(email, password), timeout=4)  # Espera máximo 4 segundos
    except TimeoutError:
         return 1
    
    if result is True:
         return 0
    else:
        return 1


if __name__ == "__main__":
     result = asyncio.run(main())
     if result is not None:
        print(result)

"username=nacho&password=123&"
"pedro&email=pedro@pedro.es&password=123"


#  pip3 install motor