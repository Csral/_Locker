#include "locker.h"
#include "shuffle.h"
#include "reshuffle.h"

#include "algorithm_registry.h"

/*

todo: Delete output file if error occurs ------------------------> (1)
todo: Add --safe-write
todo: Add a few try-catch to help with (1) but propagate them upward.

*/

void Locker::validate_configuration(const struct app_config& configuration) {

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

    if (configuration.reshuffler_opts.disabled && configuration.shuffler_opts.disabled)
        throw std::runtime_error("Both shuffler and reshuffler disabled. Nothing to do!");

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

    this->reshuffler_disabled = configuration.reshuffler_opts.disabled;
    this->shuffler_disabled = configuration.shuffler_opts.disabled;

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

void Locker::workflow(const std::vector<unsigned char>& data) {

    // todo: Better error handling.

    if (this->is_encoding) {

        std::size_t max_bits_in_workflow = data.size() * 8;

        /* First invoke shuffler */
        if (!this->shuffler_disabled) {
            shuffle::shuffle_context_t s_ctx;
            s_ctx.ptr = (void*) data.data();
            s_ctx.size_bits = max_bits_in_workflow;
            s_ctx.chunk_size_bits = this->shuffle_chunk_size;
            s_ctx.modification_factor = this->modification_factor;

            // Call shuffler upon the data.
            shuffle::dispatch_algorithm(this->shuffle_algorithm, this->is_encoding, s_ctx);
        }

        /* Now invoke reshuffler */
        if (!this->reshuffler_disabled) {

            //! Bug: Reshuffler should get 1 reshuffle block. This should be a loop.

            std::size_t bits_used_in_one_run = this->reshuffle_chunks_size * this->block_size;
            reshuffle::reshuffle_context_t r_ctx;
            
            r_ctx.buf = (void*) data.data();
            r_ctx.chunk_size_bits = this->reshuffle_chunks_size;
            r_ctx.block_size = this->block_size;
            r_ctx.seed = this->seed;
            r_ctx.extargs = this->extargs;

            std::string type = (this->type_is_bit) ? "bit" : "chunk";

            for (std::size_t ctr = 0; ctr < max_bits_in_workflow; ctr += bits_used_in_one_run) {

                r_ctx.buf = (void*) (data.data() + (ctr / 8));
                r_ctx.offset_from_previous_block = (ctr % 8);
                // Call reshuffler upon the data.
                reshuffle::dispatch_algorithm(type, this->reshuffle_algorithm, r_ctx);

            }

        }

    } else {

        // Do the inverse in decode

        std::size_t max_bits_in_workflow = data.size() * 8;

        /* First invoke reshuffler */
        if (!this->reshuffler_disabled) {

            //! Bug: Reshuffler should get 1 reshuffle block. This should be a loop.

            std::size_t bits_used_in_one_run = this->reshuffle_chunks_size * this->block_size;
            reshuffle::reshuffle_context_t r_ctx;
            
            r_ctx.buf = (void*) data.data();
            r_ctx.chunk_size_bits = this->reshuffle_chunks_size;
            r_ctx.block_size = this->block_size;
            r_ctx.seed = this->seed;
            r_ctx.extargs = this->extargs;

            std::string type = (this->type_is_bit) ? "bit" : "chunk";

            for (std::size_t ctr = 0; ctr < max_bits_in_workflow; ctr += bits_used_in_one_run) {

                r_ctx.buf = (void*) (data.data() + (ctr / 8));
                r_ctx.offset_from_previous_block = (ctr % 8);
                // Call reshuffler upon the data.
                reshuffle::dispatch_algorithm(type, this->reshuffle_algorithm, r_ctx);

            }

        }

        /* Now invoke shuffler */
        if (!this->shuffler_disabled) {
            shuffle::shuffle_context_t s_ctx;
            s_ctx.ptr = (void*) data.data();
            s_ctx.size_bits = max_bits_in_workflow;
            s_ctx.chunk_size_bits = this->shuffle_chunk_size;
            s_ctx.modification_factor = this->modification_factor;

            // Call shuffler upon the data.
            shuffle::dispatch_algorithm(this->shuffle_algorithm, this->is_encoding, s_ctx);
        }

    }

    // the data is modified.
    return;

};

void Locker::lock(void) {

    /* Encode/Decode the input file */

    // Open the input file.
    std::filesystem::path input_file_path = std::filesystem::weakly_canonical(this->input_file);
    this->input_file_handle.open(input_file_path, std::ios::in | std::ios::binary);

    if (!this->input_file_handle.is_open()) {
        throw std::runtime_error("Failed to open input file.");
    }

    std::uintmax_t file_size = std::filesystem::file_size(input_file_path);
    //? Optimize for large size by not allocating huge vector? -----> YES.

    if (file_size > this->ram_usage_allowed) {
        // we loop and run the workflow.
    } else {
        // we read it all and run the workflow once.

        const std::size_t file_buffer_size = static_cast<std::size_t>(file_size);

        std::vector<unsigned char> buffer(file_buffer_size);

        if (!this->input_file_handle.read(reinterpret_cast<char*>(buffer.data()), file_buffer_size)) {
            throw std::runtime_error("Failed to read the file.");
        }

        this->input_file_handle.close();

        // invoke the workflow
        this->workflow(buffer);
        // write the output.
        // this will change when --safe-write is added. Better to use a private method.

        std::filesystem::path output_file_path = std::filesystem::weakly_canonical(this->output_file);
        this->output_file_handler.open(output_file_path, std::ios::out | std::ios::binary);

        if (!this->output_file_handler.is_open()) {
            throw std::runtime_error("Failed to open output file.");
        }

        this->output_file_handler.write(reinterpret_cast<char*>(buffer.data()), file_buffer_size);
        this->output_file_handler.flush();
        this->output_file_handler.close();

    }

};