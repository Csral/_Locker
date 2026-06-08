#include "locker.h"
#include "shuffle.h"
#include "reshuffle.h"

#include "algorithm_registry.h"

void validate_configuration(const struct app_config& configuration) {

    //? check: What happens if user gives more bits than file size - etc. Mathematical validations basically.
    //? think of it later ig.

    if (configuration.input_file.empty())
        throw std::runtime_error("No input file provided!");

    if (!std::filesystem::exists(configuration.input_file)) {
        throw std::runtime_error("Input file not found: " + configuration.input_file);
    }

    if (!configuration.output_file.empty()) {

        std::filesystem::path input_file_path = std::filesystem::weakly_canonical(configuration.input_file);
        std::filesystem::path output_file_path = std::filesystem::weakly_canonical(configuration.output_file);

        if ((input_file_path == output_file_path) || (std::filesystem::exists(input_file_path) &&
        std::filesystem::exists(output_file_path) && std::filesystem::equivalent(input_file_path, output_file_path)))
            throw std::runtime_error("Input and output files cannot be the same!");

    }

    // all good
    return;

};


Locker::Locker(const struct app_config& configuration) {

    // Convert the configuration into local state and discard the configuration.
    // Firstly validate everything obtained.

    // todo: Add a --safe-write option which will write a ".tmp" file first.

    // Do not catch any exceptions - pass it to the upper layer.
    this->validate_configuration(configuration);

    /* Global config */
    this->input_file = configuration.input_file;
    this->ram_usage_allowed = configuration.memory_usage;
    this->is_encoding = configuration.is_encoder;

    /* Shuffler config */
    this->shuffle_chunk_size = configuration.shuffler_opts.chunk_size;
    this->shuffle_algorithm = configuration.shuffler_opts.algorithm;
    this->modification_factor = *(configuration.shuffler_opts.modification_factor);

    /* Reshuffler config */
    this->reshuffle_algorithm = configuration.reshuffler_opts.algorithm;
    this->reshuffle_chunks_size = configuration.reshuffler_opts.chunk_size;
    this->type_is_bit = (configuration.reshuffler_opts.type == "bit");
    this->block_size = configuration.reshuffler_opts.block_size;
    this->seed = *(configuration.reshuffler_opts.seed);

    // extargs are not implemented yet.

    // output file
    this->output_file = configuration.output_file;

    if (this->output_file.empty()) {
        
        std::string modifier_name = (this->is_encoding) ? "_encoded" : "_decoded";

        size_t extension_position = this->input_file.rfind('.', std::string::npos);
        if (extension_position == std::string::npos) {
            this->output_file = this->input_file + modifier_name;
        } else {
            this->output_file = this->input_file.substr(0x00U, extension_position) + modifier_name + this->input_file.substr(extension_position, std::string::npos);
        }

    }

    // Locker is setup.

};

Locker::~Locker(void) {

    // todo: delete the output file upon failure.

    if (this->input_file_handle.is_open()) {
        this->input_file_handle.close();
    }

    if (this->output_file_handler.is_open()) {
        this->output_file_handler.close();
    }

}

void Locker::lock(void) {

    /* Encode/Decode the input file */

    // Open the input file.
    std::filesystem::path input_file_path = std::filesystem::weakly_canonical(this->input_file);
    this->input_file_handle.open(input_file_path, std::ios::in | std::ios::binary);

    if (!this->input_file_handle.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }

    std::uintmax_t file_size = std::filesystem::file_size(input_file_path);
    //? Optimize for large size by not allocating huge vector? -----> YES.

    if (file_size > this->ram_usage_allowed) {
        // we loop and run the workflow.
    } else {
        // we read it all and run the workflow once.

        std::vector<unsigned char> buffer(file_size);

        if (!this->input_file_handle.read(reinterpret_cast<char*>(buffer.data()), file_size)) {
            throw std::runtime_error("Failed to read the file.");
        }

        this->input_file_handle.close();

        // make a private workflow function to easen the task.

    }
    /* First invoke Shuffler */

    // shuffle::dispatch_algorithm(this->shuffle_algorithm, this->is_encoding, NULL);

};