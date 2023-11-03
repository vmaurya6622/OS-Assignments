#include "loader.h"

Elf32_Ehdr *ehdr;
Elf32_Phdr *phdr;
int fd;
size_t entry_address;
size_t entry_offset;
void *virtual_mem;
void *oldAddress;
int (*_address)();
int pageFault = 0;
int totalPages = 0;
size_t totalMeomorySize = 0;
size_t internalFragmentation = 0;
int phdrTable[1000][2];

void loader_cleanup();
void load_and_run_elf(char** argv);
void Call_entrypoint(Elf32_Ehdr *ehdr, Elf32_Phdr *phdr);
void run_program(int (*_start)());
void SegmentationFaultHandler(int signal, siginfo_t *info, void *context);
int Ceil(int i, int k);


int main(int argc, char** argv){
	if (argc != 2) {
	printf("Usage: %s <ELF Executable> \n", argv[0]);
	exit(1);
	}

	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = SegmentationFaultHandler;

	if (sigaction(SIGSEGV, &sa, NULL) == -1) {
		perror("sigaction");
		return 1;
	}

	load_and_run_elf(argv);
	loader_cleanup();

	if (munmap(virtual_mem, totalMeomorySize) == -1){
		perror("Error unmapping memory");
		loader_cleanup();
		exit(1);
	}

	printf("Total Page Faults            : %d\n", pageFault);
	printf("Total Page Allocations       : %d\n", totalPages);
	printf("Total Memory Allocated       : %d Bytes or %f KB\n", (int)totalMeomorySize , totalMeomorySize/1000.0);
	printf("Total Internal Fragmentation : %d Bytes or %f KB\n\n", internalFragmentation , internalFragmentation/1000.0);

	return 0;
}

int Ceil(int i, int k){
    if(i % k == 0) return i / k;
    return i / k + 1;
}

void load_and_run_elf(char** argv){
	fd = open(argv[1], O_RDONLY);
	if (fd == -1){
		perror("Failed to open file");
		loader_cleanup();
		exit(1);
	}

	ehdr = (Elf32_Ehdr *) malloc(sizeof(Elf32_Ehdr));
	if(read(fd, ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)){
		perror("Error reading ELF header");
		loader_cleanup();
		exit(1);
	}

	phdr = (Elf32_Phdr *) malloc(ehdr -> e_phentsize * ehdr -> e_phnum);
    if (lseek(fd, ehdr -> e_phoff, SEEK_SET) == -1) {
        perror("lseek");
        exit(0);
    }
	if (read(fd, phdr, ehdr -> e_phentsize * ehdr -> e_phnum) != (ssize_t)(ehdr -> e_phentsize * ehdr -> e_phnum)) {
		perror("Error reading program header table");
		loader_cleanup();
		exit(1);
	}

	Call_entrypoint(ehdr, phdr);
}

void Call_entrypoint(Elf32_Ehdr *ehdr, Elf32_Phdr *phdr){

	for(int i = 0; i < ehdr->e_phnum; i++){
		phdrTable[i][0] = 0;
		phdrTable[i][1] = phdr[i].p_memsz;
	}

    printf("\nEntry Point Address : %p\n\n", (void *)ehdr -> e_entry);
	entry_offset = ehdr -> e_entry;
	entry_address = (entry_offset);
	_address = (int (*)()) entry_address;
	int (*_start)() = (int (*)())_address;
	int result = _start();
	printf("\nUser _start return value = %d\n", result);
    printf("END OF PROGRAM!\n\n");
}


void loader_cleanup()		// Function to Clean any allocated resources and memory
{
	if (ehdr != NULL)       // Clearing the space allocated to ELF header
	{
		free(ehdr);
		ehdr = NULL;
	}
	if (phdr != NULL)		// Clearing the space allocated to Program header
	{
		free(phdr);
		phdr = NULL;
	}
	if (fd != -1)			// Closing the file descriptor
	{
		close(fd);
		fd = -1;
	}
}

void SegmentationFaultHandler(int signal, siginfo_t *info, void *context) {
    ucontext_t *uc = (ucontext_t *)context;
    void *fault_address = (void *)info->si_addr;

    printf("Segmentation Fault At Address: %p\n", fault_address);

    for (int i = 0; i < ehdr->e_phnum; i++) {
        if ((int)fault_address <= phdr[i].p_vaddr + phdr[i].p_memsz && (int)fault_address >= phdr[i].p_vaddr) {

            int mem_size = phdr[i].p_memsz;
            int pageCounter = Ceil(mem_size, 4096);
            size_t allocationSize = 4096;

            if (phdrTable[i][0] == 0) {
                oldAddress = (void *)phdr[i].p_vaddr;
                if (lseek(fd, phdr[i].p_offset, SEEK_SET) == -1) {
                    perror("lseek");
                    exit(0);
                }
                phdrTable[i][0] = 1;
            }

            virtual_mem = mmap(oldAddress, allocationSize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
            if (virtual_mem == MAP_FAILED) {
                perror("mmap");
                exit(0);
            }

            int sizeUsing = phdrTable[i][1] > 4096 ? 4096 : phdrTable[i][1];
            oldAddress += (sizeUsing + 1);

            printf("SEGMENT MEMORY SIZE : %d Bytes\n", phdr[i].p_memsz);
            printf("LOADING SIZE        : %d Bytes or %f KB\n", sizeUsing, sizeUsing / 1024.0);
            printf("---------------------------------------------------------------------\n");

            if (phdrTable[i][1] - 4096 < 0 && phdrTable[i][1] > 0) {
                if (read(fd, virtual_mem, phdrTable[i][1]) == -1) {
                    perror("read");
                    exit(0);
                }
                internalFragmentation += (allocationSize - (size_t)phdrTable[i][1]);
                phdrTable[i][1] = 0;
            }

            else if (phdrTable[i][1] - 4096 > 0) {
                phdrTable[i][1] -= 4096;
                if (read(fd, virtual_mem, 4096) == -1) {
                    perror("read");
                    exit(0);
                }
            }

            pageFault++;
            totalPages++;
            totalMeomorySize += allocationSize;
        }
    }
}
