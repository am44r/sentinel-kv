FROM gcc:latest
WORKDIR /app
COPY . .
RUN mkdir -p bin data
RUN make
EXPOSE 6379
CMD ["./bin/sentinel-kv"]