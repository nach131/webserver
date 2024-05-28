#!/usr/bin/env python3

import sys
import asyncio
from motor.motor_asyncio import AsyncIOMotorClient
from urllib.parse import parse_qs

# URL de conexión a MongoDB
url = 'mongodb://root:klingon@192.168.1.20:27017'
client = AsyncIOMotorClient(url, serverSelectionTimeoutMS=4000)  # Timeout de 4 segundos
db_name = 'prueba'

def parse_query_string(query_string):
    parsed_dict = parse_qs(query_string)
    return {k: v[0] for k, v in parsed_dict.items()}

async def verify_user(username, password):
    try:
        db = client[db_name]
        coleccion = db['users']
        user = await coleccion.find_one({"username": username})
        if user and user.get("password") == password:
            return True
        return False
    except Exception as e:
        print("An error occurred:", e)
        return None

async def main():
    if len(sys.argv) != 2:
        print("Content-Type: text/plain\n")
        print("Usage: python script.py 'query_string'")
        return 1
    
    query_string = sys.argv[1]
    user_data = parse_query_string(query_string)
    
    username = user_data.get('username')
    password = user_data.get('password')

    try:
        result = await asyncio.wait_for(verify_user(username, password), timeout=4)  # Espera máximo 4 segundos
    except asyncio.TimeoutError:
        result = None

    if result is True:
        print("Content-Type: text/plain\n")
        print("0")  # Usuario verificado correctamente
    else:
        print("Content-Type: text/plain\n")
        print("1")  # Error en la verificación del usuario o timeout

if __name__ == "__main__":
    asyncio.run(main())
