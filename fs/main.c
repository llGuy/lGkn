#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>

struct initrd_header {
  uint32_t magic;
  char name[64];
  uint32_t offset;
  uint32_t length;
};

int main(int argc, char *argv[]) {
  int32_t header_count = (argc - 1) / 2;
  struct initrd_header headers[64];
  printf("size of header: %lu\n", sizeof(struct initrd_header));
  uint32_t offset = sizeof(struct initrd_header) * 64 + sizeof(int32_t);

  for (int i = 0; i < header_count; ++i) {
    printf(
      "writing file %s->%s at 0x%x\n",
      argv[i * 2 + 1], argv[i * 2 + 2], offset);
    strcpy(headers[i].name, argv[i * 2 + 2]);
    headers[i].offset = offset;

    FILE *stream = fopen(argv[i * 2 + 1], "r");
    if (stream == 0) {
      printf("Error: file not found: %s\n", argv[i * 2 + 1]);
      return 1;
    }

    fseek(stream, 0, SEEK_END);
    headers[i].length = ftell(stream);
    offset += headers[i].length;
    fclose(stream);

    headers[i].magic = 0xBF;
  }

  FILE *wstream = fopen("./initrd.img", "w");
  uint8_t *data = (uint8_t *)malloc(offset);
  fwrite(&header_count, sizeof(int), 1, wstream);
  fwrite(headers, sizeof(struct initrd_header), 64, wstream);

  for (int i = 0; i < header_count; ++i) {
    FILE *stream = fopen(argv[i * 2 + 1], "r");
    uint8_t *buf = (uint8_t *)malloc(headers[i].length);
    fread(buf, 1, headers[i].length, stream);
    fwrite(buf, 1, headers[i].length, wstream);
    fclose(stream);
    free((void *)buf);
  }

  fclose(wstream);
  free((void *)data);

  return 0;
}

