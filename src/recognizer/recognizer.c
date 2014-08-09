#include <pocketsphinx.h>

int main(int argc, char *argv[])
{
    char *filename = argc == 1 ? "/tmp/noapp_recording.pcm" : argv[1];
    FILE *fh;
    cmd_ln_t *config;
    ps_decoder_t *ps;
    int retvalue;
    char const *hyp, *uttid;
    int32 score;

    if ((fh = fopen(filename, "rb")) == NULL){
        perror("Failed to open goforward.raw");
        return 1;
    }

    if ((config = cmd_ln_init(NULL, ps_args(), TRUE,
                "-hmm", MODELDIR "/hmm/en_US/hub4wsj_sc_8k",
                "-lm", MODELDIR "/lm/turtle/turtle.DMP",
                "-dict", MODELDIR "/lm/turtle/turtle.dic",
                NULL)) == NULL){
        return 1;
    }

    if ((ps = ps_init(config)) == NULL){
        return 1;
    }

    if ((retvalue = ps_decode_raw(ps, fh, "goforward", -1)) < 0){
        return 1;
    }

    if ((hyp = ps_get_hyp(ps, &score, &uttid)) == NULL){
        return 1;
    }

    printf("\nRecognized: %s\n", hyp);

    fclose(fh);
    ps_free(ps);
    return 0;
}
