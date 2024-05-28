import asyncio
import aiohttp
from aiohttp import web
from motor.motor_asyncio import AsyncIOMotorClient
from urllib.parse import parse_qs

url = 'mongodb://root:klingon@192.168.1.20:27017'
client = AsyncIOMotorClient(url)
db_name = 'prueba'

async def parse_query_string(query_string):
    parsed_dict = parse_qs(query_string)
    return {k: v[0] for k, v in parsed_dict.items()}

async def create_user(user_data):
    try:
        db = client[db_name]
        collection = db['users']
        result = await collection.insert_one(user_data)
        return result.inserted_id
    except Exception as e:
        print("An error occurred:", e)
        return None

def doc(username):
    html = f"""
    <!DOCTYPE html>
    <html lang="en">
        <head>
            <meta charset="UTF-8">
            <title>Create User</title>
        </head>
        <body>
            <center><h1>User {username} was created</h1></center>
            <hr>
            <center>vduchi & nmota-bu</center>
        </body>
    </html>
    """
    return html

async def handle(request):
    query_string = request.query_string
    user_data = await parse_query_string(query_string)

    username = user_data.get('username')
    user_data_complete = {
        'username': user_data.get('username'),
        'email': user_data.get('email'),
        'password': user_data.get('password')
    }

    user_id = await create_user(user_data_complete)
    while user_id is None:
        user_id = await create_user(user_data_complete)
    
    html_content = doc(username)
    return web.Response(text=html_content, content_type='text/html')

app = web.Application()
app.add_routes([web.get('/', handle)])

if __name__ == "__main__":
    web.run_app(app, port=8080)
