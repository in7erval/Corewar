#include "corewar.h"

void kill(char *str) {
	ft_putstr_fd(str, 2);
	exit(0);
}

char *byte_to_chars(uint8_t byte) {
	char *str;
	char *radix = "0123456789abcdef";

	str = ft_strnew(2);
	str[1] = radix[byte % 16];
	str[0] = radix[(byte / 16) % 16];
	return (str);
}

void	print_byte(uint8_t byte) {
	char *str;

	str = byte_to_chars(byte);
	ft_putstr(str);
	free(str);
}

void 	print_nbytes(uint8_t bytes[], size_t size) {
	size_t i;

	i = 0;
	while (i < size) {
		if (i % 16 == 0 && i != 0)
			ft_putchar('\n');
		print_byte(bytes[i]);
		if (i % 2 == 1)
			ft_putchar(' ');
		i++;
	}
}

int32_t bytes_to_int32(const uint8_t *bytes, size_t size) {
	int32_t num;
	bool	minus;
	size_t 	i;

	num = 0;
	i = 0;
	minus = bytes[0] & 0x80; //1000 0000
	while (size > 0) {
		num += bytes[size - 1] << i * 8;
		i++;
		size--;
	}
	if (minus)
		num = ~num;
	return (num);

}

char *bytes_to_string(const uint8_t bytes[], size_t size) {
	char *str;
	size_t i;

	i = 0;
	str = ft_strnew(size);
	while (i < size) {
		str[i] = (char)bytes[i];
		i++;
	}
	return (str);
}

bool	check_null_bytes(const uint8_t bytes[]) {
	return bytes[0] == 0x00 && bytes[1] == 0x00 && bytes[2] == 0x00 && bytes[3] == 0x00;
}

int main(void) {
	int fd;
	int rd;
	uint8_t header[4];

	fd = open("Batman.cor", O_RDONLY);

	/* read magic header */
	rd = read(fd, &header, 4);
	if (rd < 4)
		kill("error");
	//print_nbytes(header, 4);
	int32_t num = bytes_to_int32(header, 4);
	//ft_printf("\n%d\n", num);
	if (num == COREWAR_EXEC_MAGIC)
		ft_printf("\t{bold}magic head exists{eoc}\n");

	/* read name */
	uint8_t name[128];
	rd = read(fd, &name, 128);
	if (rd < 128)
		kill("error");
	//print_nbytes(name, 128);
	char *name_str = bytes_to_string(name, 128);
	ft_printf("\nname: %s\n", name_str);
	free(name_str);

	/* read null bytes */
	uint8_t null_bytes[4];
	rd = read(fd, &null_bytes, 4);
	if (rd < 4)
		kill("error");
	if (check_null_bytes(null_bytes))
		ft_printf("\t{light}null bytes exists{eoc}\n");

	/* read code_size */
	uint8_t exec_code_size_bytes[4];
	rd = read(fd, &exec_code_size_bytes, 4);
	if (rd < 4)
		kill("error");
	int32_t exec_code_size = bytes_to_int32(exec_code_size_bytes, 4);
	ft_printf("code_size: %d\n",  exec_code_size);
	if (exec_code_size < CHAMP_MAX_SIZE)
		ft_printf("\t{light}code_size < CHAMP_MAX_SIZE (682){eoc}\n");

	/* read comment */
	uint8_t comment[2048];
	rd = read(fd, &comment, 2048);
	if (rd < 2048)
		kill("error");
	//print_nbytes(comment, 2048);
	char *comment_str = bytes_to_string(comment, 2048);
	ft_printf("comment: %s\n", comment_str);
	free(comment_str);

	/* read null bytes */
	rd = read(fd, &null_bytes, 4);
	if (rd < 4)
		kill("error");
	if (check_null_bytes(null_bytes))
		ft_printf("\t{light}null bytes exists{eoc}\n");

	/* read exec code */
	uint8_t exec_code_bytes[exec_code_size];
	rd = read(fd, &exec_code_bytes, exec_code_size);
	if (rd < exec_code_size)
		kill("error");
	ft_printf("exec code:\n");
	print_nbytes(exec_code_bytes, exec_code_size);
	ft_printf("\n");
	close(fd);

	/* print file bytes */
	ft_printf("\nFile bytes:\n");
	fd = open("Batman.cor", O_RDONLY);
	uint8_t buffer[4 + 128 + 4 + 4 + 2048 + 682];
	rd = read(fd, &buffer, 4 + 128 + 4 + 4 + 2048 + 682);
	print_nbytes(buffer, rd);
	return (0);
}
