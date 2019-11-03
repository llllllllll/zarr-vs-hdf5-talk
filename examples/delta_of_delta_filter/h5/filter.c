#include <stdint.h>
#include <limits.h>
#include <stdlib.h>

#include <H5Ipublic.h>
#include <H5public.h>
#include <H5Zpublic.h>
#include <H5Tpublic.h>

#define UNUSED(name) __attribute__((unused)) name

struct dod {
    size_t size;
    int64_t first;
    int64_t first_delta;
    int16_t delta_of_deltas[];
};

static int encode(size_t size, int64_t in[size], struct dod* out) {
    out->size = size;
    if (size == 0) {
        return 0;
    }
    out->first = in[0];
    if (size == 1) {
        return 0;
    }
    int64_t last_delta = in[1] - out->first;
    out->first_delta = last_delta;
    for (size_t ix = 2; ix < size; ++ix) {
        int64_t delta = in[ix] - in[ix - 1];
        int64_t dod = delta - last_delta;
        if (dod < INT16_MIN || dod >= INT16_MAX) {
            /* diff too large to fit in output */
            return -1;
        }
        out->delta_of_deltas[ix - 2] = dod;
        last_delta = delta;
    }

    return 0;
}

static int64_t* decode(struct dod* in) {
    int64_t* out = malloc(in->size * sizeof(int64_t));
    if (in->size == 0) {
        return out;
    }
    out[0] = in->first;
    if (in->size == 1) {
        return out;
    }
    out[1] = in->first + in->first_delta;
    if (in->size == 2) {
        return out;
    }

    int16_t last_delta = in->first_delta;
    for (size_t ix = 2; ix < in->size; ++ix) {
        int16_t dod = in->delta_of_deltas[ix - 2];
        int64_t delta = dod + last_delta;
        last_delta = delta;

        out[ix] = out[ix - 1] + delta;
    }

    return out;
}

static size_t filter(unsigned int flags,
                     size_t UNUSED(cd_nelmts),
                     const unsigned int UNUSED(cd_values)[],
                     size_t nbytes,
                     size_t *buf_size,
                     void **buf) {
    if (!(flags & H5Z_FLAG_REVERSE)) {
        size_t nelems = nbytes / sizeof(int64_t);
        size_t new_buf_size = sizeof(struct dod) + (nelems - 2) * sizeof(int16_t);
        struct dod* out = malloc(new_buf_size);

        if (encode(nelems, (int64_t*) *buf, out)) {
            return 0;
        }
        *buf = out;
        *buf_size = new_buf_size;
        return *buf_size;
    }
    else {
        struct dod* in = *buf;
        *buf = decode(in);
        *buf_size = sizeof(int64_t) * in->size;
        return *buf_size;
    }
}

static htri_t can_apply(hid_t UNUSED(dcpl_id), hid_t type_id, hid_t UNUSED(space_id)) {
    return H5Tget_class(type_id) == H5T_INTEGER &&
           H5Tget_order(type_id) == H5T_ORDER_LE &&
           H5Tget_size(type_id) == 8;
}

const int dod_filter_id = 277;

static H5Z_class2_t dod_filter = {
    .version = H5Z_CLASS_T_VERS,
    .id = dod_filter_id,
    .encoder_present = 1,
    .decoder_present = 1,
    .name = "delta_of_deltas",
    .can_apply = can_apply,
    .set_local = NULL,
    .filter = filter,
};

int dod_filter_register() {
    return H5Zregister(&dod_filter);
}
