/****************************************************************************
 * Pharos - A Real-Time Secure Operating System                             *
 * Copyright 2020 Pedro Macara and Filipe Monteiro                          *
 *                                                                          *
 * Licensed under the Apache License, Version 2.0 (the "License");          *
 * you may not use this file except in compliance with the License.         *
 * You may obtain a copy of the License at                                  *
 *                                                                          *
 *     http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                          *
 * Unless required by applicable law or agreed to in writing, software      *
 * distributed under the License is distributed on an "AS IS" BASIS,        *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 * See the License for the specific language governing permissions and      *
 * limitations under the License.                                           *
 *                                                                          *
 ****************************************************************************/


#ifndef PHAROS_DEF_H
#define PHAROS_DEF_H

/*****************************************
 * Helpful definitions of values         *
 *****************************************/

#define VALUE_512PiB                (512UL*1024UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_256PiB                (256UL*1024UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_128PiB                (128UL*1024UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_64PiB                 (64UL*1024UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_32PiB                 (32UL*1024UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_16PiB                 (16UL*1024UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_8PiB                  (8UL*1024UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_4PiB                  (4UL*1024UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_2PiB                  (2U*1024UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_1PiB                  (1024UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_512TiB                (512UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_256TiB                (256UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_128TiB                (128UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_64TiB                 (64UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_32TiB                 (32UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_16TiB                 (16UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_8TiB                  (8UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_4TiB                  (4UL*1024UL*1024UL*1024UL*1024UL)
#define VALUE_2TiB                  (2U*1024UL*1024UL*1024UL*1024UL)
#define VALUE_1TiB                  (1024UL*1024UL*1024UL*1024UL)
#define VALUE_512GiB                (512UL*1024UL*1024UL*1024UL)
#define VALUE_256GiB                (256UL*1024UL*1024UL*1024UL)
#define VALUE_128GiB                (128UL*1024UL*1024UL*1024UL)
#define VALUE_64GiB                 (64UL*1024UL*1024UL*1024UL)
#define VALUE_32GiB                 (32UL*1024UL*1024UL*1024UL)
#define VALUE_16GiB                 (16UL*1024UL*1024UL*1024UL)
#define VALUE_8GiB                  (8UL*1024UL*1024UL*1024UL)
#define VALUE_4GiB                  (4UL*1024UL*1024UL*1024UL)
#define VALUE_2GiB                  (2U*1024U*1024U*1024U)
#define VALUE_1GiB                  (1U*1024U*1024U*1024U)
#define VALUE_512MiB                (512U*1024U*1024U)
#define VALUE_256MiB                (256U*1024U*1024U)
#define VALUE_128MiB                (128U*1024U*1024U)
#define VALUE_64MiB                 (64U*1024U*1024U)
#define VALUE_32MiB                 (32U*1024U*1024U)
#define VALUE_16MiB                 (16U*1024U*1024U)
#define VALUE_8MiB                  (8U*1024U*1024U)
#define VALUE_4MiB                  (4U*1024U*1024U)
#define VALUE_2MiB                  (2U*1024U*1024U)
#define VALUE_1MiB                  (1024U*1024U)
#define VALUE_512KiB                (512U*1024U)
#define VALUE_256KiB                (256U*1024U)
#define VALUE_128KiB                (128U*1024U)
#define VALUE_64KiB                 (64U*1024U)
#define VALUE_32KiB                 (32U*1024U)
#define VALUE_16KiB                 (16U*1024U)
#define VALUE_8KiB                  (8U*1024U)
#define VALUE_4KiB                  (4U*1024U)
#define VALUE_2KiB                  (2U*1024U)
#define VALUE_1KiB                  (1024U)
#define VALUE_512B                  (512U)
#define VALUE_256B                  (256U)
#define VALUE_128B                  (128U)
#define VALUE_64B                   (64U)
#define VALUE_32B                   (32U)
#define VALUE_16B                   (16U)
#define VALUE_8B                    (8U)
#define VALUE_4B                    (4U)
#define VALUE_2B                    (2U)
#define VALUE_1B                    (1U)


/**
 * 1 Billion
 */
#define VALUE_1G                    (1000U*1000U*1000U)


/**
 * 100 Million
 */
#define VALUE_100M                  (100U * 1000U*1000U)


/**
 * 10 Million
 */
#define VALUE_10M                   (10U * 1000U*1000U)


/**
 * 1 Million
 */
#define VALUE_1M                    (1000U*1000U)


/**
 * 100 Thousand
 */
#define VALUE_100K                  (100000U)


/**
 * 10 Thousand
 */
#define VALUE_10K                   (10000U)


/**
 * 1 Thousand
 */
#define VALUE_1K                    (1000U)


/**
 * Boolean value for FALSE
 */
#define FALSE                       (0)


/**
 * Boolean value for TRUE
 */
#define TRUE                        (1U)



#define UINT32_MAX                  (0xffffffffU)        
#define UINT64_MAX                  (0xffffffffffffffffU)    


/* PHAROS_DEF_H */
#endif 
