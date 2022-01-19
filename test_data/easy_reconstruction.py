from pathlib import Path
from PIL import Image
import numpy as np
from joblib import Parallel, delayed


import colmapeasy

def main(flags):
    assert flags.image_path.exists()

    images = [
        Image.open(path).convert('L')
        for path in flags.image_path.glob('*.jpg')
    ][:4]
    
    result = Parallel(n_jobs=4, prefer="threads")(
        delayed(colmapeasy.extract_features)(
            image,
            SiftExtraction__num_threads='12',
            SiftExtraction__use_gpu='0',
        )
        for image in images
    )

    keypoints, descriptors = zip(*result)
    
    print(len(keypoints), len(descriptors))

    result2 = colmapeasy.match(
        descriptors[0], 
        descriptors[1],
    )

    print(result2)


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--image_path', type=Path)
    parser.set_defaults(image_path=Path('test_data/Keeb/images'))
    args = parser.parse_args()

    main(args)