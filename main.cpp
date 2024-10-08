#include <rdkafka.h>

#include <memory>

int main() {
  const char* brokers = "localhost:9092";

  char err[512];

  std::unique_ptr<rd_kafka_conf_t, void (*)(rd_kafka_conf_t*)> conf{
      rd_kafka_conf_new(), &rd_kafka_conf_destroy};
  if (rd_kafka_conf_set(conf.get(), "bootstrap.servers", brokers, err,
                        sizeof(err)) != RD_KAFKA_CONF_OK) {
    return 2;
  }

  std::unique_ptr<rd_kafka_t, void (*)(rd_kafka_t*)> producer{
      rd_kafka_new(RD_KAFKA_PRODUCER, conf.get(), err, sizeof(err)),
      &rd_kafka_destroy};
  if (!producer) {
    return 3;
  }
  [[maybe_unused]] auto _ = conf.release();

  rd_kafka_flush(producer.get(), -1);
  puts("Success!");

  return 0;
}
