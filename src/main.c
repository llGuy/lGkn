int main() {
	unsigned int *screen = (unsigned int *)0xb8000;
	*screen = 0x2f4b2f4f;

	return 0;
}
