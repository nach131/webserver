FROM nginx:latest

LABEL version="1.0"
LABEL description="NGINX Server"

COPY ./default.conf /etc/nginx/conf.d/default.conf

# Comando para ejecutar NGINX en primer plano
CMD ["nginx", "-g", "daemon off;"]