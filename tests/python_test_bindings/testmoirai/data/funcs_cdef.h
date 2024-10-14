extern char* get_name(void* x);
extern void* create_cat();
extern void* create_dog();
extern int put_in_dog_kenel(void* d);
extern void* create_homo_erectus();
extern void free_string(char* x);
extern void release_handle(void* ptr);
extern void release_handle_species_via_moirai(void* ptr);
extern int reference_count(void* ptr);
extern void* create_mamal_ptr(void* cat);
extern void register_exception_callback_function(const void* callback);
extern int has_callback_registered();
extern void trigger_callback();

