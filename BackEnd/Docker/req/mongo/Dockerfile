# Usa la imagen oficial de MongoDB como base
FROM mongo:4.4.18
COPY init-mongo.js /docker-entrypoint-initdb.d/mongo-init.js
CMD ["mongod", "--auth"]